#include <Util.hpp>
#include <dirent.h>
#include <errno.h>

char* UplinkStrncpyImpl(const char* file, const size_t line, char* dest, const char* src, const size_t num)
{
	const auto len = strlen(src);
	if (len >= num)
	{
		printf("\nAn Uplink strncpy Failure has occured\n"
			   "=====================================\n"
			   " Location    : %s, line %zu\n"
			   " Dest. size  : %zu\n"
			   " Source size : %zu\n"
			   " Str. Source : %s\n",
			   file, line, num, len, src);
		*(volatile int*)0 = 0;
	}
	return strncpy(dest, src, num);
}

void UplinkAssertImpl(const char* file, const size_t line, const char* conditionString, const bool condition)
{
	if (!(condition))
	{
		printf("\nAn Uplink Assertion Failure has occured\n"
			   "=======================================\n"
			   " Condition : %s\n"
			   " Location  : %s, line %zu\n",
			   conditionString, file, line);
		*(volatile int*)0 = 0;
	}
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

void PrintDArray(DArray<UplinkObject*>* array)
{
	UplinkAssert(array);

	for (auto i = 0; i < array->Size(); i++)
	{
		printf("Index = %d\n", i);

		if (!array->ValidIndex(i))
		{
			puts("Not a Valid Index");
			continue;
		}

		const auto item = array->GetData(i);

		if (!item)
		{
			puts("NULL");
			continue;
		}

		item->Print();
	}
}

bool FileReadDataIntImpl(const char* sourceFile, size_t sourceLine, void* buffer, size_t size, size_t count, FILE* file)
{
	const auto actualCount = fread(buffer, size, count, file);

	if (count != actualCount)
		printf("WARNING: FileReadDataInt, request read count is different then the actual read count, request=%zu, actual=%zu, errno=%d, "
			   "%s:%zu\n",
			   count, actualCount, errno, sourceFile, sourceLine);

	return count == actualCount;
}
