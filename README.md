# Simple Browser

A lightweight and modern web browser built with Qt WebEngine.

## Features

- **Tab Management**: Multi-tab browsing with QTabWidget
- **Custom Tab Layout**: Tab bar positioned at the top with URL navigation bar below
- **Modern UI**: Clean and intuitive interface
- **Web Navigation**: Full browsing capabilities with back, forward, reload, and stop controls
- **Custom Homepage**: Beautiful gradient homepage with quick links
- **Context Menus**: Right-click tab management (clone, close, reload)
- **Keyboard Shortcuts**: Standard browser shortcuts (Ctrl+T, Ctrl+W, etc.)

## Architecture

The browser is built using Qt6 WebEngine and consists of:

- **Browser**: Main singleton managing browser windows
- **BrowserWindow**: Main window class with menu bar and layout
- **TabWidget**: Custom QTabWidget managing multiple web tabs
- **WebView**: Custom QWebEngineView for rendering web content
- **WebPage**: Custom QWebEnginePage for page management

### Tab Layout Design

Following the Qt SimpleBrowser example, the layout is structured as:
1. **Tab Bar** (top) - Shows open tabs with close buttons
2. **URL Navigation Bar** (below tab bar) - Contains back/forward buttons, URL entry, and controls
3. **Web Content** (main area) - Displays the current webpage

## Dependencies

- Qt6 Core
- Qt6 Gui
- Qt6 Widgets
- Qt6 WebEngineWidgets

The Qt WebEngine is imported as a dependency during build time via CMake.

## Build Instructions

### Prerequisites

- CMake 3.16 or higher
- Qt6 with WebEngine module
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
cmake --build . --config Release
Release\SimpleBrowser.exe
```

### Installing Qt6 WebEngine

If Qt6 WebEngine is not installed:

**Ubuntu/Debian:**
```bash
sudo apt-get install qt6-webengine-dev qt6-webengine-dev-tools
```

**Fedora:**
```bash
sudo dnf install qt6-qtwebengine-devel
```

**macOS (Homebrew):**
```bash
brew install qt@6
```

**Windows:**
Download and install Qt6 from https://www.qt.io/download

## Usage

1. Launch the application
2. Use the homepage search box or URL bar to navigate
3. Click the **+** button or press Ctrl+T to open a new tab
4. Right-click on tabs for additional options (clone, reload, close)
5. Use navigation buttons (←, →, ⟳, ✕) to control page navigation

## License

This project is open source and available for use and modification.
