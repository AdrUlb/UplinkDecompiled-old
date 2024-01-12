#pragma once

namespace BinReloc
{
	enum class InitError
	{
		/** Cannot allocate memory. */
		NoMemory,
		/** Unable to open /proc/self/maps; see errno for details. */
		CannotOpenMaps,
		/** Unable to read from /proc/self/maps; see errno for details. */
		CannotReadMaps,
		/** The file format of /proc/self/maps is invalid; kernel bug? */
		InvalidMaps
	};

	char* FindExe(InitError* error);
} // namespace BinReloc
