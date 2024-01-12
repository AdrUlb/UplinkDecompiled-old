#pragma once

#include <cstdint>
#include <cstdio>

constexpr uintptr_t PAGESIZE = 4096;
constexpr uintptr_t mainAddr = 0x004A12B0;

const auto file_stdoutPtr = (FILE**)0x007B2F30;
#define file_stdout (*file_stdoutPtr)

const auto RunUplinkExceptionHandling = (void (*)(void))0x004A0F60;
const auto hSignalSIGFPE = (void (*)(int))0x004A1210;
const auto hSignalSIGPIPE = (void (*)(int))0x004A11C0;

const auto RunUplink = (void (*)(int, char**))0x004A10D0;
