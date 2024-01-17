#include <Util.hpp>
#include <dirent.h>
#include <errno.h>

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
