#pragma once

#include <cstdint>
#include <cstdio>

constexpr uintptr_t PAGESIZE = 4096;
constexpr uintptr_t mainAddr = 0x080FF260;

const auto file_stdoutPtr = (FILE**)0x08206A24;
#define file_stdout (*file_stdoutPtr)

const auto RunUplinkExceptionHandling = (void (*)(void))0x080FEE90;
const auto hSignalSIGFPE = (void (*)(int))0x080FF1C0;
const auto hSignalSIGPIPE = (void (*)(int))0x080FF170;

const auto RunUplink = (void (*)(int, char**))0x080FF050;
