#pragma once

#include <cstdio>
#include <unistd.h>

namespace RedShirt
{
	void Initialise(const char* path);
	void CleanUp();
	unsigned int FileCheckSum(FILE* file, void* buffer, size_t size);
	FILE* FileOpen(const char* path, const char* modes);
	void FileClose(const char* filePath, FILE* file);
	unsigned int FileCheckSum(FILE* file, void* buffer, size_t size);
	bool FileEncryptedNoVerify(const char* path);
	bool FileEncrypted(const char* path);
	bool EncryptFile(const char* path);
} // namespace RedShirt
