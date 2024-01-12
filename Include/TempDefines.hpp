#pragma once

typedef void Options;
typedef void Network;
typedef void MainMenu;
typedef void PhoneDialler;

#if defined(__x86_64__)
#include <TempDefines_x64.hpp>
#elif defined(__i386__)
#include <TempDefines_x86.hpp>
#else
#error Platform not supported.
#endif
