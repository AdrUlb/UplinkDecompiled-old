#include <RedShirt.hpp>
#include <TempDefines.hpp>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <dirent.h>

static inline size_t HashResultSize()
{
	return 20;
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

void RedShirt::Initialise(const char* path)
{
	char* pcVar1;
	size_t tempDirLen;

	strcpy(gRsAppPath, path);
	strcpy(gRsTempDir, "/tmp/uplink-XXXXXX");
	pcVar1 = mkdtemp(gRsTempDir);
	if (!pcVar1)
	{
		puts("Failed to make temporary directory");
		abort();
	}

	tempDirLen = strlen(gRsTempDir);
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

	if (strcmp(signature, "REDSHIRT") == 0)
	{
		fclose(file);
		return true;
	}

	if (strcmp(signature, "REDSHRT2") == 0)
	{
		const auto hashResultSize = HashResultSize();
		const auto expectedHash = new unsigned char[hashResultSize];

		auto success = false;

		if (fread(expectedHash, hashResultSize, 1, file) == 1)
		{
			const auto actualHash = new unsigned char[hashResultSize];
			const auto hashByteCount = RsFileCheckSum(file, actualHash, hashResultSize);

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

	fclose(file);
	return false;
}
