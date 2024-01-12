cmake_minimum_required(VERSION 3.22)

if(NOT DEFINED GAME_DIR)
	message(FATAL_ERROR "The variable GAME_DIR must be set to the Uplink game directory.")
endif()

if(NOT DEFINED GAME_ARCH)
	message(FATAL_ERROR "The variable GAME_ARCH must be set.")
endif()

if (${GAME_ARCH} STREQUAL "x64")
	set(EXE_ARCH_NAME x86_64)
elseif(${GAME_ARCH} STREQUAL "x86")
	set(EXE_ARCH_NAME x86)
else()
	message(FATAL_ERROR "Value '${GAME_ARCH}' for GAME_ARCH is not a supported architecture.")
endif()

project(UplinkDecompiled)

add_library(UplinkDecompiled SHARED
	Source/LibMain.cpp
	Source/Main.cpp)

target_include_directories(UplinkDecompiled PRIVATE Include)

if(${GAME_ARCH} STREQUAL "x86")
set_target_properties(UplinkDecompiled PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
endif()

add_custom_target(run
	DEPENDS UplinkDecompiled
	COMMAND LD_PRELOAD=$<TARGET_FILE:UplinkDecompiled> ${GAME_DIR}/uplink.bin.${EXE_ARCH_NAME})
