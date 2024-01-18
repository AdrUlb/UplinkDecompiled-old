#pragma once
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <unistd.h>

char* UplinkStrncpyImpl(const char* sourceFile, int sourceLine, char* dest, const char* src, size_t num);

void UplinkAssertImpl(const char* sourceFile, int sourceLine, const char* conditionString, bool condition);

template <typename... T>
void UplinkSnprintfImpl(const char* sourceFile, int line, char* buffer, size_t bufferSize, const char* format, T... params);

void UplinkAbortImpl(const char* sourceFile, const int sourceLine, const char* message);

char* GetFilePath(const char* path);

void EmptyDirectory(const char* path);

bool FileReadDataImpl(const char* sourceFile, int sourceLine, void* buffer, size_t size, size_t count, FILE* file);

bool LoadDynamicStringImpl(const char* sourceFile, const int sourceLine, char*& buffer, FILE* file);

#define UplinkStrncpy(dest, src, num) UplinkStrncpyImpl(__FILE__, __LINE__, dest, src, num)

#define UplinkAssert(condition) UplinkAssertImpl(__FILE__, __LINE__, #condition, condition)

#define UplinkSnprintf(buffer, bufferSize, format, ...) UplinkSnprintfImpl(__FILE__, __LINE__, buffer, bufferSize, format, __VA_ARGS__)

#define UplinkAbort(message) UplinkAbortImpl(__FILE__, __LINE__, message)

#define FileReadData(buffer, size, count, file) FileReadDataImpl(__FILE__, __LINE__, buffer, size, count, file)

#define LoadDynamicString(buffer, file) LoadDynamicStringImpl(__FILE__, __LINE__, buffer, file)

static inline bool DoesFileExist(const char* path)
{
	return access(path, 0) == 0;
}

static inline void MakeDirectory(char* path)
{
	mkdir(path, 0700);
}

#include <Util.tpp>
