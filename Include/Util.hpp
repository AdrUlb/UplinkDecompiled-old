#pragma once
#include <cstring>

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
				   " Dest. size  : %d\n"                                                                                                   \
				   " Source size : %d\n"                                                                                                   \
				   " Str. Source : %s\n",                                                                                                  \
				   __FILE__, __LINE__, num, len, src);                                                                                     \
			*(int*)0 = 0;                                                                                                                  \
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
			*(int*)0 = 0;                                                                                                                  \
		}                                                                                                                                  \
	}

#define UplinkSnprintf(buffer, bufferSize, format, ...)                                                                                    \
	{                                                                                                                                      \
		if (snprintf(buffer, bufferSize, format, __VA_ARGS__) >= bufferSize)                                                               \
		{                                                                                                                                  \
			printf("\nAn Uplink snprintf Failure has occured\n======================================\n"                                    \
				   " Location    : %s, line %d\n"                                                                                          \
				   " Buffer size : %d\n"                                                                                                   \
				   " Format      : %s\n"                                                                                                   \
				   " Buffer      : %s\n",                                                                                                  \
				   __FILE__, __LINE__, bufferSize, format, buffer);                                                                        \
			*(int*)0 = 0;                                                                                                                  \
		}                                                                                                                                  \
		buffer[bufferSize - 1] = 0;                                                                                                        \
	}
