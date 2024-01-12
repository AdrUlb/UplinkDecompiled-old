#include <TempDefines.hpp>
#include <cstdint>
#include <sys/mman.h>

static inline uintptr_t GetPageFromAddress(uintptr_t address)
{
	return address & ~(PAGESIZE - 1);
}

template <typename T> static void WriteCode(uintptr_t address, T value)
{
	const auto ptr = reinterpret_cast<T*>(address);

	const auto page1 = reinterpret_cast<void*>(GetPageFromAddress(address));
	const auto page2 = reinterpret_cast<void*>(GetPageFromAddress(address + sizeof(T) - 1));

	mprotect(page1, PAGESIZE, PROT_WRITE);

	if (page1 != page2)
		mprotect(page2, PAGESIZE, PROT_WRITE);

	*ptr = value;

	mprotect(page1, PAGESIZE, PROT_EXEC | PROT_READ);

	if (page1 != page2)
		mprotect(page2, PAGESIZE, PROT_EXEC | PROT_READ);
}

static void WriteJump(uintptr_t from, const uintptr_t to)
{
#if defined(__x86_64__)
	WriteCode<uint8_t>(from++, 0xFF);
	WriteCode<uint8_t>(from++, 0x25);
	WriteCode<uint8_t>(from++, 0x00);
	WriteCode<uint8_t>(from++, 0x00);
	WriteCode<uint8_t>(from++, 0x00);
	WriteCode<uint8_t>(from++, 0x00);
	WriteCode<uint64_t>(from++, to);
#elif defined(__i386__)
	WriteCode<uint8_t>(from, 0xE9);
	WriteCode<uint32_t>(from + 1, to - from - 5);
#else
#error Platform not supported.
#endif
}

extern int main(int argc, char* argv[]);

__attribute__((constructor)) static void libmain()
{
	WriteJump(mainAddr, reinterpret_cast<uintptr_t>(main));
}
