#pragma once

#include <App.hpp>
#include <Options.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdio>

static constexpr uintptr_t pagesize = 4096;
static constexpr uintptr_t mainAddr = 0x080FF260;

static const auto file_stdoutPtr = (FILE**)0x08206A24;
#define file_stdout (*file_stdoutPtr)

static const auto gAppPtr = (App**)0x08206304;
#define gApp (*gAppPtr)

static const auto gRsAppPath = (char*)0x08205DE0;
static const auto gRsTempDir = (char*)0x08205EE0;

static const auto gRsInitialisedPtr = (bool*)0x082070A9;
#define gRsInitialised (*gRsInitialisedPtr)

static const auto gWindowScaleXPtr = (float*)0x08204948;
#define gWindowScaleX (*gWindowScaleXPtr)

static const auto gWindowScaleYPtr = (float*)0x08204944;
#define gWindowScaleY (*gWindowScaleYPtr)

static const auto baseCompileDate = (const char*)0x081AD138;
static const auto baseCompileTime = (const char*)0x081AD12F;

static const auto RunUplinkExceptionHandling = (void (*)(void))0x080FEE90;
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
static const auto DeleteBTreeData = (void (*)(BTree<UplinkObject*>*))0x080587A0;
static const auto DeleteDArrayDataD = (void (*)(DArray<UplinkObject*>*))0x080591D0;
static const auto EclGetAccurateTime = (double (*)(void))0x08177BB0;
static const auto LoadBTree = (bool (*)(BTree<UplinkObject*>*, FILE*))0x08059460;

static const auto BglCloseAllFiles = (void (*)(void))0x0817DF90;

static const auto App__Close = (void (*)(App*))0x0804D5B0;
static const auto App__Print = (void (*)(App*))0x0804D060;
static const auto App__Update = (void (*)(App*))0x0804DAD0;
static const auto App__Initialise = (void (*)(App*))0x0804EBD0;

static const auto Options__Save = (void (*)(Options*, FILE*))0x080FAC20;
static const auto Options__Print = (void (*)(Options*))0x080FABE0;
static const auto Options__CreateDefaultOptions = (void (*)(Options*))0x080FB980;

static const auto Network__Update = (void (*)(Network*))0x080F8730;
static const auto NetworkClient__Update = (void (*)(NetworkClient*))0x080F9480;
static const auto NetworkServer__Update = (void (*)(NetworkServer*))0x080F9C10;

static const auto MainMenu__Update = (bool (*)(MainMenu*))0x080EC330;

static const auto StubImplTcp4uInit = (int (*)(void))0x0817F2B8;
static const auto StubImplTcp4uCleanup = (int (*)(void))0x0817F30D;
