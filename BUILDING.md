# Building and Running SimpleBrowser

## Quick Start

### Prerequisites
- CMake 3.16 or higher
- Qt6 with WebEngine module
- C++17 compatible compiler
- Git

### Clone and Build

```bash
# Clone the repository
git clone https://github.com/sriail/simple-qt.git
cd simple-qt

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make -j$(nproc)

# Run the browser
./SimpleBrowser
```

## Installation of Dependencies

### Ubuntu/Debian (22.04+)

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    qt6-base-dev \
    qt6-webengine-dev \
    qt6-webengine-dev-tools \
    libqt6webenginecore6
```

### Fedora

```bash
sudo dnf install -y \
    gcc-c++ \
    cmake \
    qt6-qtbase-devel \
    qt6-qtwebengine-devel
```

### Arch Linux

```bash
sudo pacman -S \
    base-devel \
    cmake \
    qt6-base \
    qt6-webengine
```

### macOS (Homebrew)

```bash
brew install cmake qt@6
```

### Windows

1. Install Visual Studio 2019 or later with C++ support
2. Download and install CMake from https://cmake.org/download/
3. Download and install Qt6 from https://www.qt.io/download
4. Make sure Qt6 WebEngine module is selected during installation

**Building on Windows:**

```cmd
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019"
cmake --build . --config Release
Release\SimpleBrowser.exe
```

## Project Structure

```
simple-qt/
├── CMakeLists.txt          # Main CMake configuration
├── README.md               # Project overview
├── BUILDING.md            # This file
├── src/                   # Source code
│   ├── main.cpp           # Application entry point
│   ├── browser.h/cpp      # Browser singleton
│   ├── browserwindow.h/cpp # Main window with UI
│   ├── tabwidget.h/cpp    # Tab management
│   ├── webpage.h/cpp      # Custom web page
│   └── webview.h/cpp      # Custom web view
└── resources/             # Application resources
    ├── resources.qrc      # Qt resource file
    └── homepage.html      # Default homepage

```

## Features Implemented

### Tab Management
- Multi-tab browsing with QTabWidget
- Tab bar positioned at top (North position)
- Close buttons on each tab
- Context menu on tabs (right-click):
  - Reload Tab
  - Clone Tab
  - Close Other Tabs
  - Close Tab
  - Reload All Tabs

### Navigation
- URL bar with auto-complete
- Back/Forward navigation buttons
- Reload and Stop buttons
- New tab button (+)
- Keyboard shortcuts:
  - Ctrl+T: New tab
  - Ctrl+W: Close tab
  - Ctrl+Q: Quit
  - F5: Reload
  - Esc: Stop loading

### UI Layout (As Requested)
The layout follows the requirements with:
1. **Tab Bar** at the top (using QTabWidget::North)
2. **URL Navigation Bar** below the tab bar
3. **Web Content** in the main area

This is different from typical browsers but matches the requested specification.

### Homepage
- Beautiful gradient design
- Search functionality
- Quick links to popular sites
- Keyboard shortcuts reminder

## Qt WebEngine Integration

The browser uses Qt WebEngine which is imported as a build dependency via CMake:

```cmake
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets)
target_link_libraries(SimpleBrowser PRIVATE Qt6::WebEngineWidgets)
```

Qt WebEngine is based on Chromium and provides:
- Full HTML5, CSS3, and JavaScript support
- Hardware acceleration
- Modern web standards compliance
- Security features

## Troubleshooting

### "Could NOT find Qt6" Error

Make sure Qt6 is installed and in your PATH. You may need to set `CMAKE_PREFIX_PATH`:

```bash
cmake -DCMAKE_PREFIX_PATH=/path/to/qt6 ..
```

### WebEngine Not Found

Install the Qt6 WebEngine package separately:

```bash
# Ubuntu/Debian
sudo apt-get install qt6-webengine-dev

# Or download from Qt website
```

### Display Issues on Linux

If running in a headless environment or without X11:

```bash
export DISPLAY=:0
# or use Xvfb
Xvfb :99 -screen 0 1280x1024x24 &
export DISPLAY=:99
./SimpleBrowser
```

### GPU Acceleration Errors

If you see GPU errors, they're usually non-fatal. The browser will fall back to software rendering.

## Development

### Building for Development

```bash
# Debug build
mkdir build-debug
cd build-debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
```

### Code Style

The project follows Qt coding conventions:
- CamelCase for class names
- camelCase for method names
- m_ prefix for member variables
- Clear and descriptive names

## License

This project is open source and available for educational and commercial use.

## References

- Qt WebEngine Documentation: https://doc.qt.io/qt-6/qtwebengine-index.html
- SimpleBrowser Example: https://doc.qt.io/qt-6/qtwebengine-webenginewidgets-simplebrowser-example.html
- Qt Documentation: https://doc.qt.io/qt-6/
