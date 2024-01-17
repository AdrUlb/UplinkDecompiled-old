#pragma once
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

// Original decompiled sources suggest a redundant null terminator was written to the end of the string.
// This is not required as strncpy pads the end of the buffer with zeros until num bytes have been written.
#define UplinkStrncpy(dest, src, num)                                                                                                      \
	{                                                                                                                                      \
		const auto len = strlen(src);                                                                                                      \
		if (len >= num)                                                                                                                    \
		{                                                                                                                                  \
			printf("\nAn Uplink strncpy Failure has occured\n"                                                                             \
				   "=====================================\n"                                                                               \
				   " Location    : %s, line %d\n"                                                                                          \
				   " Dest. size  : %zu\n"                                                                                                  \
				   " Source size : %zu\n"                                                                                                  \
				   " Str. Source : %s\n",                                                                                                  \
				   __FILE__, __LINE__, num, len, src);                                                                                     \
			*(volatile int*)0 = 0;                                                                                                         \
		}                                                                                                                                  \
		strncpy(dest, src, num);                                                                                                           \
	}

#define UplinkAssert(condition)                                                                                                            \
	{                                                                                                                                      \
		if (!(condition))                                                                                                                  \
		{                                                                                                                                  \
			printf("\nAn Uplink Assertion Failure has occured\n"                                                                           \
				   "=======================================\n"                                                                             \
				   " Condition : %s\n"                                                                                                     \
				   " Location  : %s, line %d\n",                                                                                           \
				   #condition, __FILE__, __LINE__);                                                                                        \
			*(volatile int*)0 = 0;                                                                                                         \
		}                                                                                                                                  \
	}

#define UplinkSnprintf(buffer, bufferSize, format, ...)                                                                                    \
	{                                                                                                                                      \
		if (bufferSize < 0 || (size_t)snprintf(buffer, bufferSize, format, __VA_ARGS__) >= (size_t)bufferSize)                             \
		{                                                                                                                                  \
			printf("\nAn Uplink snprintf Failure has occured\n======================================\n"                                    \
				   " Location    : %s, line %d\n"                                                                                          \
				   " Buffer size : %zu\n"                                                                                                  \
				   " Format      : %s\n"                                                                                                   \
				   " Buffer      : %s\n",                                                                                                  \
				   __FILE__, __LINE__, bufferSize, format, buffer);                                                                        \
			*(volatile int*)0 = 0;                                                                                                         \
		}                                                                                                                                  \
		buffer[bufferSize - 1] = 0;                                                                                                        \
	}

#include <DArray.hpp>
#include <UplinkObject.hpp>

static inline bool DoesFileExist(const char* path)
{
	return access(path, 0) == 0;
}

static inline void MakeDirectory(char* path)
{
	mkdir(path, 0700);
}

char* GetFilePath(const char* path);

void EmptyDirectory(const char* path);

void PrintDArray(DArray<UplinkObject*>* array);

bool FileReadDataIntImpl(const char* sourceFile, size_t sourceLine, void* buffer, size_t size, size_t count, FILE* file);

#define FileReadDataInt(buffer, size, count, file) FileReadDataIntImpl(__FILE__, __LINE__, buffer, size, count, file)
