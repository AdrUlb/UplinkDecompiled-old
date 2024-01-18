#include <RedShirt.hpp>
#include <TempDefines.hpp>
#include <algorithm>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <openssl/evp.h>

typedef bool (*ReadHeaderCallback)(FILE*);
typedef bool (*WriteHeaderCallback)(FILE*);
typedef bool (*WriteChecksumCallback)(FILE*);
typedef void (*WriteDataCallback)(unsigned char*, unsigned int);

static inline size_t HashResultSize()
{
	return 20;
}

static EVP_MD_CTX* HashInitial()
{
	const auto context = EVP_MD_CTX_new();

	EVP_DigestInit_ex(context, EVP_sha1(), nullptr);
	return context;
}

static void HashData(EVP_MD_CTX* context, unsigned char* data, unsigned int length)
{
	EVP_DigestUpdate(context, data, length);
}

static unsigned int HashFinal(EVP_MD_CTX* context, void* buffer, unsigned int size)
{
	unsigned char digest[20];

	unsigned int outlen;
	EVP_DigestFinal_ex(context, digest, &outlen);
	EVP_MD_CTX_free(context);

	// FIXME? the original game fucked something up with the endianness, this is replicating that
	for (auto ptr = digest; ptr < digest + 20; ptr += 4)
	{
		auto temp = ptr[0];
		ptr[0] = ptr[3];
		ptr[3] = temp;

		temp = ptr[1];
		ptr[1] = ptr[2];
		ptr[2] = temp;
	}

	size = std::min<unsigned int>(outlen, size);
	memcpy(buffer, digest, size);

	return size;
}

static inline void DeleteDirectory(const char* path)
{
	rmdir(path);
}

static const char* Basename(const char* path)
{
	while (true)
	{
		auto dirSepStart = strchr(path, '/');
		if (!dirSepStart)
		{
			dirSepStart = strchr(path, '\\');
			if (!dirSepStart)
				return path;
		}

		path = dirSepStart + 1;
	}
}

static bool FileExists(const char* path)
{
	const auto file = fopen(path, "r");

	if (file)
	{
		fclose(file);
		return true;
	}

	return false;
}

static bool noHeader(FILE* file)
{
	(void)file;
	return true;
}

static bool filterStream(FILE* file, FILE* filteredFile, WriteDataCallback writeDataCallback)
{
	unsigned char readBuffer[0x4000];

	size_t readCount;
	size_t writeCount;

	do
	{
		readCount = fread(readBuffer, 1, sizeof(readBuffer), file);

		if (readCount == 0)
			return true;

		writeDataCallback(readBuffer, readCount);

		writeCount = fwrite(readBuffer, 1, readCount, filteredFile);
	} while (readCount <= writeCount);

	return false;
}

static bool readRsEncryptedHeader(FILE* file)
{
	char signature[9];

	if (fread(signature, 9, 1, file) != 1)
		return false;

	if (strcmp(signature, "REDSHRT2") == 0)
	{
		const auto hashResultSize = HashResultSize();
		const auto hash = new char[hashResultSize];

		auto success = fread(hash, hashResultSize, 1, file) == 1;

		if (hash)
			delete[] hash;

		return success;
	}

	return strcmp(signature, "REDSHIRT") == 0;
}

static bool writeRsEncryptedHeader(FILE* file)
{
	if (fwrite("REDSHRT2", 9, 1, file) != 1)
		return false;

	const auto hashSize = HashResultSize();
	const auto hash = new unsigned char[hashSize];

	memset(hash, 0, hashSize);

	auto success = fwrite(hash, hashSize, 1, file) == 1;

	delete[] hash;

	return success;
}

static bool writeRsEncryptedCheckSum(FILE* file)
{
	const auto hashSize = HashResultSize();
	const auto buffer = new unsigned char[hashSize];

	fseek(file, hashSize + 9, 0);

	auto success = false;

	if (hashSize != RedShirt::FileCheckSum(file, buffer, hashSize))
	{
		fseek(file, 9, 0);
		success = fwrite(buffer, hashSize, 1, file) == 1;
	}

	delete[] buffer;

	return success;
}

static void decryptBuffer(unsigned char* buffer, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		buffer[i] += 0x80;
}

static void encryptBuffer(unsigned char* buffer, unsigned int size)
{
	for (unsigned int i = 0; i < size; i++)
		buffer[i] += 0x80;
}

static bool filterFile(const char* path, const char* filteredPath, ReadHeaderCallback readHeaderCallback,
					   WriteHeaderCallback writeHeaderCallback, WriteChecksumCallback writeChecksumCallback,
					   WriteDataCallback writeDataCallback)
{

	auto file = fopen(path, "rb");

	if (!file)
		return false;

	if (!readHeaderCallback(file))
	{
		printf("redshirt: failed to read header!");
		fclose(file);
		return false;
	}

	auto filteredFile = fopen(filteredPath, "w+b");

	if (!filteredFile)
	{
		fclose(file);
		return false;
	}

	if (!writeHeaderCallback(filteredFile))
	{
		printf("redshirt: failed to write header!");
		fclose(file);
		fclose(filteredFile);
		remove(filteredPath);
		return false;
	}

	if (!filterStream(file, filteredFile, writeDataCallback))
	{
		printf("redshirt: failed to write containning bytes!");
		fclose(file);
		fclose(filteredFile);
		remove(filteredPath);
		return false;
	}

	if (!writeChecksumCallback(filteredFile))
	{
		printf("redshirt: failed to write checksum!");
		fclose(file);
		fclose(filteredFile);
		remove(filteredPath);
		return false;
	}

	fclose(file);
	fclose(filteredFile);
	return true;
}

bool filterFileInPlace(const char* path, const char* filteredSuffix, ReadHeaderCallback readHeaderCallback,
					   WriteHeaderCallback writeHeaderCallback, WriteChecksumCallback writeChecksumCallback,
					   WriteDataCallback writeDataCallback)
{
	char buffer[0x100];

	sprintf(buffer, "%s%s", path, filteredSuffix);
	if (filterFile(path, buffer, readHeaderCallback, writeHeaderCallback, writeChecksumCallback, writeDataCallback))
	{
		remove(path);
		rename(buffer, path);
		return true;
	}
	puts("Redshirt ERROR : Failed to write output file");
	return false;
}

void RedShirt::Initialise(const char* path)
{
	strcpy(gRsAppPath, path);
	strcpy(gRsTempDir, "/tmp/uplink-XXXXXX");
	if (!mkdtemp(gRsTempDir))
	{
		puts("Failed to make temporary directory");
		abort();
	}

	const auto tempDirLen = strlen(gRsTempDir);
	gRsTempDir[tempDirLen] = '/';
	gRsTempDir[tempDirLen + 1] = 0;

	gRsInitialised = true;
	atexit(CleanUp);
}

void RedShirt::CleanUp()
{
	if (!gRsInitialised)
		return;

	gRsInitialised = false;

	auto dir = opendir(gRsTempDir);

	char buffer[0x100];
	if (dir)
	{
		for (auto dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
		{
			snprintf(buffer, sizeof(buffer), "%s%s", gRsTempDir, dirEntry->d_name);
			remove(buffer);
		}

		closedir(dir);
	}

	DeleteDirectory(gRsTempDir);
	BglCloseAllFiles();
	return;
}

unsigned int RedShirt::FileCheckSum(FILE* file, void* buffer, size_t size)
{
	unsigned char readBuffer[0x4000];

	const auto hashContext = HashInitial();

	size_t readCount;
	while (true)
	{
		readCount = fread(readBuffer, 1, sizeof(readBuffer), file);

		if (readCount == 0)
			break;

		HashData(hashContext, readBuffer, readCount);
	}

	return HashFinal(hashContext, buffer, size);
}

FILE* RedShirt::FileOpen(const char* path, const char* modes)
{
	char buffer[0x100];

	if (!FileExists(path))
		return nullptr;

	if (!FileEncrypted(path))
		return fopen(path, modes);

	sprintf(buffer, "%s%s.d", gRsTempDir, Basename(path));

	if (!filterFile(path, buffer, readRsEncryptedHeader, noHeader, noHeader, decryptBuffer))
	{
		puts("Redshirt ERROR : Failed to write to output file");
		return nullptr;
	}

	return fopen(buffer, modes);
}

void RedShirt::FileClose(const char* filePath, FILE* file)
{
	char buffer[0x100];

	fclose(file);
	sprintf(buffer, "%s.d", filePath);
	remove(buffer);
}

bool RedShirt::FileEncryptedNoVerify(const char* path)
{
	const auto file = fopen(path, "rb");

	if (!file)
		return false;

	const auto ret = readRsEncryptedHeader(file);

	fclose(file);

	return ret;
}

bool RedShirt::FileEncrypted(const char* path)
{
	char signature[0x10];

	const auto file = fopen(path, "rb");

	if (!file)
		return false;

	if (fread(signature, 9, 1, file) != 1)
	{
		fclose(file);
		return false;
	}

	// File is RedShirt 1
	if (strcmp(signature, "REDSHIRT") == 0)
	{
		fclose(file);
		return true;
	}

	// File is neither RedShirt 1 nor 2
	if (strcmp(signature, "REDSHRT2") != 0)
	{
		fclose(file);
		return false;
	}

	// File is RedShirt 2

	const auto hashResultSize = HashResultSize();
	const auto expectedHash = new unsigned char[hashResultSize];

	auto success = false;

	if (fread(expectedHash, hashResultSize, 1, file) == 1)
	{
		const auto actualHash = new unsigned char[hashResultSize];
		const auto hashByteCount = RedShirt::FileCheckSum(file, actualHash, hashResultSize);

		if (hashByteCount != 0)
			success = memcmp(expectedHash, actualHash, hashByteCount) == 0;

		if (actualHash)
			delete[] actualHash;
	}

	if (expectedHash)
		delete[] expectedHash;

	fclose(file);
	return success;
}

bool RedShirt::EncryptFile(const char* path)
{
	return filterFileInPlace(path, ".e", noHeader, writeRsEncryptedHeader, writeRsEncryptedCheckSum, encryptBuffer);
}
