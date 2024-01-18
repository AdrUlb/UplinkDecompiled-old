#include <cstddef>

template <typename... T> void UplinkSnprintfImpl(const char* sourceFile, const int sourceLine, char* buffer, size_t bufferSize, const char* format, T... params)
{
	if (snprintf(buffer, bufferSize, format, params...) >= (int)bufferSize)
	{
		printf("\n"
			   "An Uplink snprintf Failure has occured\n"
			   "======================================\n"
			   " Location    : %s, line %d\n"
			   " Buffer size : %zu\n"
			   " Format      : %s\n"
			   " Buffer      : %s\n",
			   sourceFile, sourceLine, bufferSize, format, buffer);
		*(volatile int*)0 = 0;
	}
	buffer[bufferSize - 1] = 0;
}
