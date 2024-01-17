#pragma once
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

char* UplinkStrncpyImpl(const char* file, size_t line, char* dest, const char* src, size_t num);

void UplinkAssertImpl(const char* file, size_t line, const char* conditionString, bool condition);

#define UplinkStrncpy(dest, src, num) UplinkStrncpyImpl(__FILE__, __LINE__, dest, src, num)

#define UplinkAssert(condition) UplinkAssertImpl(__FILE__, __LINE__, #condition, condition)

#define UplinkSnprintf(buffer, bufferSize, format, ...)                                                                                    \
	{                                                                                                                                      \
		if (bufferSize < 0 || (size_t)snprintf(buffer, bufferSize, format, __VA_ARGS__) >= (size_t)bufferSize)                             \
		{                                                                                                                                  \
			printf("\n"                                                                                                                    \
				   "An Uplink snprintf Failure has occured\n"                                                                              \
				   "======================================\n"                                                                              \
				   " Location    : %s, line %d\n"                                                                                          \
				   " Buffer size : %zu\n"                                                                                                  \
				   " Format      : %s\n"                                                                                                   \
				   " Buffer      : %s\n",                                                                                                  \
				   __FILE__, __LINE__, bufferSize, format, buffer);                                                                        \
			*(volatile int*)0 = 0;                                                                                                         \
		}                                                                                                                                  \
		buffer[bufferSize - 1] = 0;                                                                                                        \
	}

#define UplinkAbort(message)                                                                                                               \
	{                                                                                                                                      \
		printf("\n"                                                                                                                        \
			   "Uplink has been forced to Abort\n"                                                                                         \
			   "===============================\n"                                                                                         \
			   " Message   : %s\n"                                                                                                         \
			   " Location  : %s, line %d\n",                                                                                               \
			   message, __FILE__, __LINE__);                                                                                               \
		*(volatile int*)0 = 0;                                                                                                             \
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
