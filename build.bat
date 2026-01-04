@echo off
REM Simple Qt Browser Build Script
REM This script automates the build process by creating the build directory
REM and running CMake configuration
REM Use --no-install flag to skip automatic dependency installation

setlocal enabledelayedexpansion

REM Parse command line arguments
set AUTO_INSTALL=1
:parse_args
if "%~1"=="" goto args_done
if /i "%~1"=="--no-install" (
    set AUTO_INSTALL=0
)
shift
goto parse_args
:args_done

echo Simple Qt Browser - Build Script
echo =================================
echo.

REM Track if dependencies are missing
set MISSING_DEPS=0
set CMAKE_OK=0
set QT6_OK=0
set COMPILER_OK=0

REM ============================================
REM Function to check CMake version
REM ============================================
:check_cmake
echo Checking CMake...
where cmake >nul 2>&1
if %errorlevel% neq 0 (
    echo [ERROR] CMake is not installed.
    set MISSING_DEPS=1
    set CMAKE_OK=0
    goto :eof
)

REM Get CMake version (optional - for display only)
for /f "tokens=3" %%v in ('cmake --version 2^>nul ^| findstr "^cmake"') do set CMAKE_VERSION=%%v

REM Simple version check - just verify it exists and runs
cmake --version >nul 2>&1
if %errorlevel% equ 0 (
    if defined CMAKE_VERSION (
        echo [SUCCESS] CMake found: !CMAKE_VERSION!
    ) else (
        echo [SUCCESS] CMake found
    )
    set CMAKE_OK=1
) else (
    echo [ERROR] CMake version check failed
    set MISSING_DEPS=1
    set CMAKE_OK=0
)
goto :eof

REM ============================================
REM Function to check Qt6
REM ============================================
:check_qt6
echo Checking Qt6...

REM Try to find qmake6 or qmake
set QMAKE=
where qmake6 >nul 2>&1
if %errorlevel% equ 0 (
    set QMAKE=qmake6
) else (
    where qmake >nul 2>&1
    if %errorlevel% equ 0 (
        set QMAKE=qmake
    )
)

if not defined QMAKE (
    echo [ERROR] Qt6 is not installed (qmake not found^).
    echo Please ensure Qt6 bin directory is in your PATH.
    set MISSING_DEPS=1
    set QT6_OK=0
    goto :eof
)

REM Check Qt version
for /f "delims=" %%v in ('!QMAKE! -query QT_VERSION 2^>nul') do set QT_VERSION=%%v

if not defined QT_VERSION (
    echo [ERROR] Could not determine Qt version.
    set MISSING_DEPS=1
    set QT6_OK=0
    goto :eof
)

echo !QT_VERSION! | findstr /R "^6\." >nul
if %errorlevel% neq 0 (
    echo [ERROR] Qt6 is not installed (found Qt !QT_VERSION!^).
    set MISSING_DEPS=1
    set QT6_OK=0
    goto :eof
)

echo [SUCCESS] Qt !QT_VERSION! found
set QT6_OK=1
goto :eof

REM ============================================
REM Function to check for C++ compiler
REM ============================================
:check_compiler
echo Checking for C++ compiler...

REM Check for Visual Studio
where cl.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo [SUCCESS] Visual Studio compiler found
    set COMPILER_OK=1
    goto :eof
)

REM Check for MinGW
where g++.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo [SUCCESS] MinGW compiler found
    set COMPILER_OK=1
    goto :eof
)

REM Check for MSYS2/MinGW64
where mingw32-make.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo [SUCCESS] MSYS2/MinGW toolchain found
    set COMPILER_OK=1
    goto :eof
)

REM Check for clang
where clang++.exe >nul 2>&1
if %errorlevel% equ 0 (
    echo [SUCCESS] Clang compiler found
    set COMPILER_OK=1
    goto :eof
)

echo [WARNING] No C++ compiler found in PATH
echo Note: CMake will search for compilers in standard locations
set COMPILER_OK=1
goto :eof

REM ============================================
REM Function to automatically install dependencies
REM ============================================
:auto_install
echo.
echo Attempting automatic installation of dependencies...
echo.

REM Try Chocolatey first
where choco >nul 2>&1
if %errorlevel% equ 0 (
    echo Using Chocolatey to install dependencies...
    echo Installing CMake...
    choco install cmake -y >nul 2>&1
    if %errorlevel% neq 0 (
        echo [WARNING] CMake installation via Chocolatey failed
    )
    
    echo Installing Qt6...
    choco install qt6 -y >nul 2>&1
    if %errorlevel% neq 0 (
        echo [WARNING] Qt6 installation via Chocolatey failed
    )
    
    echo [SUCCESS] Chocolatey installation completed
    echo Please restart your command prompt to update PATH
    goto :eof
)

REM Try Winget if Chocolatey is not available
where winget >nul 2>&1
if %errorlevel% equ 0 (
    echo Using Winget to install dependencies...
    echo Installing CMake...
    winget install -e --id Kitware.CMake --silent --accept-package-agreements --accept-source-agreements >nul 2>&1
    if %errorlevel% neq 0 (
        echo [WARNING] CMake installation via Winget failed
    )
    
    echo Installing Qt6...
    winget install -e --id Qt.Qt.6 --silent --accept-package-agreements --accept-source-agreements >nul 2>&1
    if %errorlevel% neq 0 (
        echo [WARNING] Qt6 installation via Winget may have failed or requires user interaction
        echo You may need to complete the Qt installation manually
    )
    
    echo [SUCCESS] Winget installation completed
    echo Please restart your command prompt to update PATH
    goto :eof
)

REM Neither package manager is available
echo [WARNING] Neither Chocolatey nor Winget is available
echo Automatic installation is not possible without a package manager
echo.
echo Please install Chocolatey or use Winget:
echo   Chocolatey: https://chocolatey.org/install
echo   Winget: Built into Windows 10/11 (may need to be enabled)
goto :eof

REM ============================================
REM Function to print installation instructions
REM ============================================
:print_install_instructions
echo.
echo ======================================
echo Missing Dependencies Installation Guide
echo ======================================
echo.
echo Option 1: Using Chocolatey (Recommended^)
echo   choco install cmake
echo   choco install qt6
echo.
echo Option 2: Using Winget
echo   winget install Kitware.CMake
echo   winget install Qt.Qt.6
echo.
echo Option 3: Manual Installation
echo   CMake: https://cmake.org/download/
echo   Qt6:   https://www.qt.io/download-qt-installer
echo.
echo For C++ Compiler (if needed^):
echo   - Visual Studio: https://visualstudio.microsoft.com/downloads/
echo     (Select "Desktop development with C++" workload^)
echo   - MinGW-w64: https://www.mingw-w64.org/
echo   - MSYS2: https://www.msys2.org/
echo.
echo After installation, make sure to:
echo   1. Add Qt6 bin directory to PATH (e.g., C:\Qt\6.x.x\{compiler}\bin^)
echo      where {compiler} is your toolchain like msvc2019_64, mingw_64, etc.
echo   2. Add CMake bin directory to PATH
echo   3. Restart your command prompt
echo.
goto :eof

REM ============================================
REM Main script execution
REM ============================================

echo Checking build dependencies...
echo.

call :check_cmake
call :check_qt6
call :check_compiler

echo.

REM If dependencies are missing, show instructions and exit
if %MISSING_DEPS% equ 1 (
    if %AUTO_INSTALL% equ 1 (
        call :auto_install
        
        echo.
        echo Please restart your command prompt to refresh PATH environment variable.
        echo Then run this build script again to verify dependencies were installed correctly.
        echo.
        echo If automatic installation failed, you can:
        echo   1. Try running this script again with administrator privileges
        echo   2. Install dependencies manually (instructions below)
        echo   3. Use --no-install flag to skip automatic installation
        echo.
        call :print_install_instructions
        exit /b 1
    ) else (
        call :print_install_instructions
        echo [ERROR] Automatic installation is disabled.
        echo Please install the missing dependencies manually and try again.
        exit /b 1
    )
)

echo [SUCCESS] All dependencies are satisfied!
echo.

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

endlocal
