#pragma once

#include <cstdint>
#include <cstdio>

constexpr uintptr_t PAGESIZE = 4096;
constexpr uintptr_t mainAddr = 0x004A12B0;

const auto file_stdoutPtr = (FILE**)0x007B2F30;
#define file_stdout (*file_stdoutPtr)

const auto RunUplinkExceptionHandling = (void (*)(void))0x004A0F60;
const auto Init_App = (void (*)(const char*))0x004A0BE0;
const auto Init_Options = (void (*)(int, char**))0x004A0590;
const auto VerifyLegitAndCodeCardCheck = (bool(*)(void))0x004A07E0;
const auto Load_Data = (bool(*)(void))0x004A04A0;
const auto Init_Game = (void(*)(void))0x004A0390;
const auto Init_Graphics = (void(*)(void))0x004A01E0;
const auto Init_Fonts = (void(*)(void))0x0049FFA0;
const auto Init_Sound = (void(*)(void))0x004A0340;
const auto Init_Music = (void(*)(void))0x004A0210;
const auto Init_OpenGL = (void(*)(void))0x0049FEF0;
const auto Run_MainMenu = (void(*)(void))0x0049FD40;
const auto Run_Game = (void(*)(void))0x0049FD30;
const auto Cleanup_Uplink = (void(*)(void))0x0049FCA0;
