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
cmake ..
if %errorlevel% neq 0 exit /b %errorlevel%

REM Build the project
echo Building the project...
cmake --build .
if %errorlevel% neq 0 exit /b %errorlevel%

echo.
echo Build complete!
echo Run the application with: SimpleBrowser.exe
