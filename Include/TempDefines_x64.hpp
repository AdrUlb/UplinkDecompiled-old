#pragma once

#include <App.hpp>
#include <cstdint>
#include <cstdio>

static constexpr uintptr_t PAGESIZE = 4096;
static constexpr uintptr_t mainAddr = 0x004A12B0;

static const auto file_stdoutPtr = (FILE**)0x007B2F30;
#define file_stdout (*file_stdoutPtr)

static const auto gAppPtr = (App**)0x007B2710;
#define gApp (*gAppPtr)

static const auto baseCompileDate = (const char*)0x005448F8;
static const auto baseCompileTime = (const char*)0x005448EF;

static const auto RunUplinkExceptionHandling = (void (*)(void))0x004A0F60;
static const auto Init_Options = (void (*)(int, char**))0x004A0590;
static const auto VerifyLegitAndCodeCardCheck = (bool (*)(void))0x004A07E0;
static const auto Load_Data = (bool (*)(void))0x004A04A0;
static const auto Init_Game = (void (*)(void))0x004A0390;
static const auto Init_Graphics = (void (*)(void))0x004A01E0;
static const auto Init_Fonts = (void (*)(void))0x0049FFA0;
static const auto Init_Sound = (void (*)(void))0x004A0340;
static const auto Init_Music = (void (*)(void))0x004A0210;
static const auto Init_OpenGL = (void (*)(void))0x0049FEF0;
static const auto Run_MainMenu = (void (*)(void))0x0049FD40;
static const auto Run_Game = (void (*)(void))0x0049FD30;
static const auto Cleanup_Uplink = (void (*)(void))0x0049FCA0;

static const auto RsInitialise = (void (*)(const char*))0x005127D0;

static const auto App__Close = (void (*)(App*))0x00407230;
static const auto App__Print = (void (*)(App*))0x00406D30;
static const auto App__Update = (void (*)(App*))0x004076F0;
static const auto App__Initialise = (void (*)(App*))0x004085E0;
