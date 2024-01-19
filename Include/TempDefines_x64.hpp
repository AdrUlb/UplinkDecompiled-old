#pragma once

#include <App.hpp>
#include <Bungle.hpp>
#include <Options.hpp>
#include <cstddef>
#include <cstdint>
#include <cstdio>

static constexpr uintptr_t pagesize = 4096;
static constexpr uintptr_t mainAddr = 0x004A12B0;

static const auto file_stdoutPtr = (FILE**)0x007B2F30;
#define file_stdout (*file_stdoutPtr)

static const auto gAppPtr = (App**)0x007B2710;
#define gApp (*gAppPtr)

static const auto gRsAppPath = (char*)0x007B2000;
static const auto gRsTempDir = (char*)0x007B2100;

static const auto gRsInitialisedPtr = (bool*)0x007B3895;
#define gRsInitialised (*gRsInitialisedPtr)

static const auto gWindowScaleXPtr = (float*)0x007B08C8;
#define gWindowScaleX (*gWindowScaleXPtr)

static const auto gWindowScaleYPtr = (float*)0x007B08C4;
#define gWindowScaleY (*gWindowScaleYPtr)

static const auto gFilesPtr = (BTree<Bungle::LocalFileHeader*>*)0x007B38C0;
#define gFiles (*gFilesPtr)

static const auto baseCompileDate = (const char*)0x005448F8;
static const auto baseCompileTime = (const char*)0x005448EF;

static const auto RunUplinkExceptionHandling = (void (*)(void))0x004A0F60;
static const auto VerifyLegitAndCodeCardCheck = (bool (*)(void))0x004A07E0;
// static const auto Load_Data = (bool (*)(void))0x004A04A0;
static const auto Init_Game = (void (*)(void))0x004A0390;
static const auto Init_Graphics = (void (*)(void))0x004A01E0;
static const auto Init_Fonts = (void (*)(void))0x0049FFA0;
static const auto Init_Sound = (void (*)(void))0x004A0340;
static const auto Init_Music = (void (*)(void))0x004A0210;
static const auto Init_OpenGL = (void (*)(void))0x0049FEF0;
static const auto Run_MainMenu = (void (*)(void))0x0049FD40;
static const auto Run_Game = (void (*)(void))0x0049FD30;
static const auto DeleteBTreeData = (void (*)(BTree<UplinkObject*>*))0x004111F0;
static const auto DeleteDArrayDataD = (void (*)(DArray<UplinkObject*>*))0x00411B10;
static const auto EclGetAccurateTime = (double (*)(void))0x0050DBC0;
static const auto CreateUplinkObject = (UplinkObject * (*)(int))0x0040FF10;

static const auto BglOpenZipFile = (bool (*)(FILE* file, const char*, const char*))0x00513EF0;
static const auto BglCloseAllFiles = (void (*)(void))0x00513740;
static const auto BglSlashify = (void (*)(char*))0x00513C00;

static const auto App__Close = (void (*)(App*))0x00407230;
static const auto App__Print = (void (*)(App*))0x00406D30;
static const auto App__Update = (void (*)(App*))0x004076F0;
static const auto App__Initialise = (void (*)(App*))0x004085E0;

static const auto Options__Save = (void (*)(Options*, FILE*))0x0049D170;
static const auto Options__Print = (void (*)(Options*))0x0049D140;
static const auto Options__CreateDefaultOptions = (void (*)(Options*))0x0049DE30;

static const auto Network__Update = (void (*)(Network*))0x0049AFD0;
static const auto NetworkClient__Update = (void (*)(NetworkClient*))0x0049BBD0;
static const auto NetworkServer__Update = (void (*)(NetworkServer*))0x0049C2C0;

static const auto MainMenu__Update = (bool (*)(MainMenu*))0x00490BD0;

static const auto StubImplTcp4uInit = (int (*)(void))0x00514944;
static const auto StubImplTcp4uCleanup = (int (*)(void))0x0051499B;
