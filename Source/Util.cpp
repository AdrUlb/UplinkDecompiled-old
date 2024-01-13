#include <Util.hpp>
#include <dirent.h>

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

	for (auto dirEntry = readdir(dir); dirEntry != nullptr; dirEntry = readdir(dir))
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
