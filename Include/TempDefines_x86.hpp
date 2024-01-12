#pragma once

#include <cstdint>
#include <cstdio>

constexpr uintptr_t PAGESIZE = 4096;
constexpr uintptr_t mainAddr = 0x080FF260;

const auto file_stdoutPtr = (FILE**)0x08206A24;
#define file_stdout (*file_stdoutPtr)

const auto RunUplinkExceptionHandling = (void (*)(void))0x080FEE90;
const auto Init_App = (void (*)(const char*))0x080FEAA0;
const auto Init_Options = (void (*)(int, char**))0x080FE410;
const auto VerifyLegitAndCodeCardCheck = (bool(*)(void))0x080FE6A0;
const auto Load_Data = (bool(*)(void))0x080FE310;
const auto Init_Game = (void(*)(void))0x080FE1D0;
const auto Init_Graphics = (void(*)(void))0x080FDFC0;
const auto Init_Fonts = (void(*)(void))0x080FDD70;
const auto Init_Sound = (void(*)(void))0x080FE170;
const auto Init_Music = (void(*)(void))0x080FDFF0;
const auto Init_OpenGL = (void(*)(void))0x080FDC90;
const auto Run_MainMenu = (void(*)(void))0x080FDA70;
const auto Run_Game = (void(*)(void))0x080FDA60;
const auto Cleanup_Uplink = (void(*)(void))0x080FD9C0;
