# Simple Qt Browser

A lightweight, modern implementation of the Qt WebEngine browser.

## Features

- Tabbed browsing with QTabWidget
- Navigation controls (Back, Forward, Reload, Home)
- URL bar with DuckDuckGo search integration
- Clean, minimalist homepage
- Error and settings pages
- Qt WebEngine backend

## Build Instructions

### Prerequisites

- CMake 3.16 or later
- Qt 6.x with WebEngine module
- C++17 compatible compiler

**Windows-specific requirements:**
- One of the following build tool chains:
  - Visual Studio 2019 or later (with C++ Desktop Development workload)
  - MinGW-w64
  - MSYS2 with mingw-w64 toolchain

**Note for Windows users:** The build script will automatically try to detect and use an available build toolchain. If you encounter errors, ensure one of the above is properly installed and in your PATH.

### Quick Build (Recommended)

The easiest way to build the project is using the provided build scripts, which will automatically install missing dependencies:

**Linux/macOS:**
```bash
./build.sh
```

**Windows:**
```bash
build.bat
```

These scripts will:
1. Automatically detect and install missing dependencies (CMake, Qt6)
2. Create the build directory
3. Configure CMake
4. Compile the project

**Note:** The scripts will attempt to install dependencies automatically using your system's package manager:
- Linux: apt-get (Ubuntu/Debian), dnf (Fedora/RHEL), or pacman (Arch)
- macOS: Homebrew
- Windows: Chocolatey or Winget

If you want to skip automatic installation and install dependencies manually, use the `--no-install` flag:
```bash
./build.sh --no-install    # Linux/macOS
build.bat --no-install     # Windows
```

### Manual Build

If you prefer to build manually:

**Linux/macOS:**
```bash
mkdir build
cd build
cmake ..
make
./SimpleBrowser
```

**Windows:**
```bash
mkdir build
cd build
cmake ..
cmake --build .
SimpleBrowser.exe
```

## Dependencies

The project uses Qt6 WebEngine which will be imported during build time:
- Qt6::Core
- Qt6::Gui
- Qt6::Widgets
- Qt6::WebEngineWidgets

## Project Structure

```
simple-qt/
├── src/
│   ├── main.cpp         # Application entry point
│   ├── browser.cpp/h    # Main browser window
│   └── webview.cpp/h    # Web view widget
├── html/
│   ├── home.html        # Homepage with search
│   ├── error.html       # Error page
│   └── settings.html    # Settings page
├── resources.qrc        # Qt resources file
└── CMakeLists.txt       # Build configuration
```

## Usage

- Click '+' button or Ctrl+T to open new tabs
- Use navigation buttons for browsing
- Enter URLs in the address bar or search with DuckDuckGo
- Click Home button to return to the homepage
