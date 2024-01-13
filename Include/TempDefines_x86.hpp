#pragma once

#include <App.hpp>
#include <cstdint>
#include <cstdio>

static constexpr uintptr_t PAGESIZE = 4096;
static constexpr uintptr_t mainAddr = 0x080FF260;

static const auto file_stdoutPtr = (FILE**)0x08206A24;
#define file_stdout (*file_stdoutPtr)

static const auto gAppPtr = (App**)0x08206304;
#define gApp (*gAppPtr)

static const auto baseCompileDate = (const char*)0x081AD138;
static const auto baseCompileTime = (const char*)0x081AD12F;

static const auto RunUplinkExceptionHandling = (void (*)(void))0x080FEE90;
static const auto Init_Options = (void (*)(int, char**))0x080FE410;
static const auto VerifyLegitAndCodeCardCheck = (bool (*)(void))0x080FE6A0;
static const auto Load_Data = (bool (*)(void))0x080FE310;
static const auto Init_Game = (void (*)(void))0x080FE1D0;
static const auto Init_Graphics = (void (*)(void))0x080FDFC0;
static const auto Init_Fonts = (void (*)(void))0x080FDD70;
static const auto Init_Sound = (void (*)(void))0x080FE170;
static const auto Init_Music = (void (*)(void))0x080FDFF0;
static const auto Init_OpenGL = (void (*)(void))0x080FDC90;
static const auto Run_MainMenu = (void (*)(void))0x080FDA70;
static const auto Run_Game = (void (*)(void))0x080FDA60;
static const auto Cleanup_Uplink = (void (*)(void))0x080FD9C0;

static const auto RsInitialise = (void (*)(const char*))0x0817CF70;

static const auto App__Close = (void (*)(App*))0x0804D5B0;
static const auto App__Print = (void (*)(App*))0x0804D060;
static const auto App__Update = (void (*)(App*))0x0804DAD0;
static const auto App__Initialise = (void (*)(App*))0x0804EBD0;
