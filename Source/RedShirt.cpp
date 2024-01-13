#include <RedShirt.hpp>
#include <TempDefines.hpp>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <dirent.h>

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
