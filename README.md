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

### Building on Linux/macOS

```bash
mkdir build
cd build
cmake ..
make
./SimpleBrowser
```

### Building on Windows

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
