@echo off
REM Simple Qt Browser Build Script
REM This script automates the build process by creating the build directory
REM and running CMake configuration

echo Simple Qt Browser - Build Script
echo =================================

REM Create build directory if it doesn't exist
if not exist "build" (
    echo Creating build directory...
    mkdir build
) else (
    echo Build directory already exists.
)

REM Navigate to build directory
cd build

REM Run CMake configuration
echo Running CMake configuration...
echo.
echo NOTE: This script requires a C++ compiler and build tools to be installed.
echo Common options:
echo   - Visual Studio (with C++ Desktop Development workload)
echo   - MinGW-w64
echo   - MSYS2 with mingw-w64 toolchain
echo.

REM Try to auto-detect and use an appropriate generator
REM First, try default (Visual Studio if available)
cmake .. 2>nul
if %errorlevel% equ 0 goto build_project

REM If default fails, try MinGW Makefiles
echo Default generator failed. Trying MinGW Makefiles...
cmake -G "MinGW Makefiles" .. 2>nul
if %errorlevel% equ 0 goto build_project

REM If MinGW fails, try Ninja
echo MinGW Makefiles not available. Trying Ninja...
cmake -G "Ninja" .. 2>nul
if %errorlevel% equ 0 goto build_project

REM If all fail, run cmake without suppressing errors to show the actual error
echo All generators failed. Running cmake to show errors...
cmake ..
if %errorlevel% neq 0 (
    echo.
    echo ERROR: CMake configuration failed.
    echo Please install one of the following:
    echo   1. Visual Studio with C++ Desktop Development workload
    echo   2. MinGW-w64 (https://www.mingw-w64.org/)
    echo   3. MSYS2 with mingw-w64 toolchain
    echo.
    exit /b %errorlevel%
)

:build_project

REM Build the project
echo.
echo CMake configuration successful!
echo Building the project...
cmake --build .
if %errorlevel% neq 0 exit /b %errorlevel%

echo.
echo Build complete!
echo Run the application with: SimpleBrowser.exe
