// Adapted from BinReloc by Hongli Lai

#include <BinReloc.hpp>

#include <climits>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

char* BinReloc::FindExe(InitError* error)
{
	char *path, *path2, *line, *result;
	size_t buf_size;
	ssize_t size;
	struct stat stat_buf;
	FILE* f;

	/* Read from /proc/self/exe (symlink) */
	if (sizeof(path) > SSIZE_MAX)
		buf_size = SSIZE_MAX - 1;
	else
		buf_size = PATH_MAX - 1;
	path = (char*)malloc(buf_size);
	if (path == NULL)
	{
		/* Cannot allocate memory. */
		if (error)
			*error = InitError::NoMemory;
		return NULL;
	}
	path2 = (char*)malloc(buf_size);
	if (path2 == NULL)
	{
		/* Cannot allocate memory. */
		if (error)
			*error = InitError::NoMemory;
		free(path);
		return NULL;
	}

	strncpy(path2, "/proc/self/exe", buf_size - 1);

	while (1)
	{
		int i;

		size = readlink(path2, path, buf_size - 1);
		if (size == -1)
		{
			/* Error. */
			free(path2);
			break;
		}

		/* readlink() success. */
		path[size] = '\0';

		/* Check whether the symlink's target is also a symlink.
		 * We want to get the final target. */
		i = stat(path, &stat_buf);
		if (i == -1)
		{
			/* Error. */
			free(path2);
			break;
		}

		/* stat() success. */
		if (!S_ISLNK(stat_buf.st_mode))
		{
			/* path is not a symlink. Done. */
			free(path2);
			return path;
		}

		/* path is a symlink. Continue loop and resolve this. */
		strncpy(path, path2, buf_size - 1);
	}

	/* readlink() or stat() failed; this can happen when the program is
	 * running in Valgrind 2.2. Read from /proc/self/maps as fallback. */

	buf_size = PATH_MAX + 128;
	line = (char*)realloc(path, buf_size);
	if (line == NULL)
	{
		/* Cannot allocate memory. */
		free(path);
		if (error)
			*error = InitError::NoMemory;
		return NULL;
	}

	f = fopen("/proc/self/maps", "r");
	if (f == NULL)
	{
		free(line);
		if (error)
			*error = InitError::CannotOpenMaps;
		return NULL;
	}

	/* The first entry should be the executable name. */
	result = fgets(line, (int)buf_size, f);
	if (result == NULL)
	{
		fclose(f);
		free(line);
		if (error)
			*error = InitError::CannotReadMaps;
		return NULL;
	}

	/* Get rid of newline character. */
	buf_size = strlen(line);
	if (buf_size <= 0)
	{
		/* Huh? An empty string? */
		fclose(f);
		free(line);
		if (error)
			*error = InitError::InvalidMaps;
		return NULL;
	}
	if (line[buf_size - 1] == 10)
		line[buf_size - 1] = 0;

	/* Extract the filename; it is always an absolute path. */
	path = strchr(line, '/');

	/* Sanity check. */
	if (strstr(line, " r-xp ") == nullptr || path == nullptr)
	{
		fclose(f);
		free(line);
		if (error)
			*error = InitError::InvalidMaps;
		return nullptr;
	}

	path = strdup(path);
	free(line);
	fclose(f);
	return path;
}
