#pragma once

#include <cstdio>
#include <unistd.h>

namespace RedShirt
{
	void Initialise(const char* path);
	void CleanUp();
	FILE* FileOpen(const char* path, const char* modes);
	void FileClose(const char* filePath, FILE* file);
	bool FileEncryptedNoVerify(const char* path);
	bool FileEncrypted(const char* path);
} // namespace RedShirt
