#include <Util.hpp>
#include <dirent.h>
#include <cstdint>
#include <errno.h>

char* UplinkStrncpyImpl(const char* sourceFile, const int sourceLine, char* dest, const char* src, const size_t num)
{
	const auto len = strlen(src);
	if (len >= num)
	{
		printf("\nAn Uplink strncpy Failure has occured\n"
			   "=====================================\n"
			   " Location    : %s, line %d\n"
			   " Dest. size  : %zu\n"
			   " Source size : %zu\n"
			   " Str. Source : %s\n",
			   sourceFile, sourceLine, num, len, src);
		*(volatile int*)0 = 0;
	}
	return strncpy(dest, src, num);
}

void UplinkAssertImpl(const char* sourceFile, const int sourceLine, const char* conditionString, const bool condition)
{
	if (!(condition))
	{
		printf("\nAn Uplink Assertion Failure has occured\n"
			   "=======================================\n"
			   " Condition : %s\n"
			   " Location  : %s, line %d\n",
			   conditionString, sourceFile, sourceLine);
		*(volatile int*)0 = 0;
	}
}

void UplinkAbortImpl(const char* sourceFile, const int sourceLine, const char* message)
{
	printf("\n"
		   "Uplink has been forced to Abort\n"
		   "===============================\n"
		   " Message   : %s\n"
		   " Location  : %s, line %d\n",
		   message, sourceFile, sourceLine);
	*(volatile int*)0 = 0;
}

bool FileReadDataImpl(const char* sourceFile, const int sourceLine, void* buffer, size_t size, size_t count, FILE* file)
{
	const auto actualCount = fread(buffer, size, count, file);

	if (count != actualCount)
		printf("WARNING: FileReadDataInt, request read count is different then the actual read count, request=%zu, actual=%zu, errno=%d, "
			   "%s:%d\n",
			   count, actualCount, errno, sourceFile, sourceLine);

	return count == actualCount;
}

bool LoadDynamicStringImpl(const char* sourceFile, const int sourceLine, char*& buffer, FILE* file)
{
	buffer = nullptr;
	int32_t length;
	if (!FileReadData(&length, sizeof(length), 1, file))
		return false;

	if (length == -1)
		return true;

	if (length > 0x4000)
	{
		printf("WARNING: LoadDynamicString, size appears to be wrong, size=%d, %s:%d\n", length, sourceFile, sourceLine);
		return false;
	}

	buffer = new char[length + 1];
	if (!FileReadData(buffer, length, 1, file))
	{
		buffer[length] = 0;
		return false;
	}
	buffer[length] = 0;

	return true;
}

char* GetFilePath(const char* path)
{
	UplinkAssert(path);

	const auto bufferSize = strlen(path) + 1;
	auto buffer = new char[bufferSize];
	if (bufferSize != 0)
		UplinkStrncpy(buffer, path, bufferSize);

	auto dirSepStart = strrchr(buffer, '/');
	if (dirSepStart)
	{
		dirSepStart[1] = 0;
		return buffer;
	}

	dirSepStart = strrchr(buffer, '\\');
	if (dirSepStart)
	{
		dirSepStart[1] = 0;
		return buffer;
	}

	if (buffer)
		delete[] buffer;

	return strdup("./");
}

void EmptyDirectory(const char* path)
{
	char buffer[0x100];

	auto dir = opendir(path);

	if (!dir)
		return;

	for (auto dirEntry = readdir(dir); dirEntry; dirEntry = readdir(dir))
	{
		if (strcmp(dirEntry->d_name, ".") == 0)
			continue;

		if (strcmp(dirEntry->d_name, "..") == 0)
			continue;

		UplinkSnprintf(buffer, sizeof(buffer), "%s%s", path, dirEntry->d_name);
		unlink(buffer);
	}

	closedir(dir);
}
