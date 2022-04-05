cmake_minimum_required(VERSION 3.14)

project(OpenCL_GameOfLife)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    include("C:\Users\paulb\Travail\vcpkg\scripts\buildsystems\vcpkg.cmake")
endif()
if(UNIX AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET x64-linux)
endif()
if(WIN32 AND NOT DEFINED VCPKG_TARGET_TRIPLET)
    set(VCPKG_TARGET_TRIPLET x64-windows)
endif()
if(WIN32)
    add_compile_options("/std:c++latest")
    # Needed for gprc.
    add_compile_options("/D WINVER=0x0A00")
    add_compile_options("/D _WIN32_WINNT=0x0A00")
endif(WIN32)

find_package(OpenCL REQUIRED)

add_executable(OpenCL_GameOfLife
	OpenCL_GameOfLife/GameOfLife.cpp
	OpenCL_GameOfLife/GameOfLifeOCL.cpp
	OpenCL_GameOfLife/KernelCode.cl
)

target_link_libraries(OpenCL_GameOfLife
  PUBLIC
	OpenCL::OpenCL	
)