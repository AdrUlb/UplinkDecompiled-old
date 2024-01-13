#pragma once

#include <unistd.h>

namespace RedShirt
{
	void Initialise(const char* path);
	void CleanUp();

	static inline void DeleteDirectory(char* path)
	{
		rmdir(path);
	}
} // namespace RedShirt
