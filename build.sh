#!/bin/bash

# Simple Qt Browser Build Script
# This script automates the build process by creating the build directory
# and running CMake configuration

echo "Simple Qt Browser - Build Script"
echo "================================="
echo ""

# Color codes for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Track if dependencies are missing
MISSING_DEPS=0

# Function to print error messages
print_error() {
    echo -e "${RED}ERROR:${NC} $1"
}

# Function to print warning messages
print_warning() {
    echo -e "${YELLOW}WARNING:${NC} $1"
}

# Function to print success messages
print_success() {
    echo -e "${GREEN}SUCCESS:${NC} $1"
}

# Function to detect OS
detect_os() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        if [ -f /etc/os-release ]; then
            . /etc/os-release
            echo "$ID"
        else
            echo "linux"
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        echo "macos"
    else
        echo "unknown"
    fi
}

# Function to check CMake version
check_cmake() {
    echo "Checking CMake..."
    if ! command -v cmake &> /dev/null; then
        print_error "CMake is not installed."
        MISSING_DEPS=1
        return 1
    fi
    
    CMAKE_VERSION=$(cmake --version | head -n1 | grep -oE '[0-9]+\.[0-9]+\.[0-9]+' | head -n1)
    REQUIRED_VERSION="3.16.0"
    
    if [ "$(printf '%s\n' "$REQUIRED_VERSION" "$CMAKE_VERSION" | sort -V | head -n1)" = "$REQUIRED_VERSION" ]; then
        print_success "CMake $CMAKE_VERSION found (>= $REQUIRED_VERSION required)"
        return 0
    else
        print_error "CMake version $CMAKE_VERSION is too old (>= $REQUIRED_VERSION required)"
        MISSING_DEPS=1
        return 1
    fi
}

# Function to check Qt6
check_qt6() {
    echo "Checking Qt6..."
    
    # Try to find qmake6 or qmake
    QMAKE=""
    if command -v qmake6 &> /dev/null; then
        QMAKE="qmake6"
    elif command -v qmake &> /dev/null; then
        QMAKE="qmake"
    fi
    
    if [ -z "$QMAKE" ]; then
        print_error "Qt6 is not installed (qmake not found)."
        MISSING_DEPS=1
        return 1
    fi
    
    # Check Qt version
    QT_VERSION=$($QMAKE -query QT_VERSION 2>/dev/null || echo "")
    if [[ ! "$QT_VERSION" =~ ^6\. ]]; then
        print_error "Qt6 is not installed (found Qt $QT_VERSION)."
        MISSING_DEPS=1
        return 1
    fi
    
    print_success "Qt $QT_VERSION found"
    
    # Check for required Qt6 modules
    QT_INSTALL_LIBS=$($QMAKE -query QT_INSTALL_LIBS)
    MISSING_MODULES=""
    
    # Note: We check for basic Qt6 installation, CMake will do detailed component checking
    if [ ! -d "$QT_INSTALL_LIBS" ]; then
        print_warning "Qt6 libraries directory not found at $QT_INSTALL_LIBS"
        MISSING_DEPS=1
        return 1
    fi
    
    print_success "Qt6 components appear to be installed"
    return 0
}

# Function to print installation instructions
print_install_instructions() {
    OS=$(detect_os)
    echo ""
    echo "======================================"
    echo "Missing Dependencies Installation Guide"
    echo "======================================"
    echo ""
    
    case "$OS" in
        ubuntu|debian|linuxmint)
            echo "For Ubuntu/Debian-based systems, run:"
            echo ""
            echo "  sudo apt-get update"
            echo "  sudo apt-get install cmake qt6-base-dev qt6-webengine-dev"
            echo ""
            ;;
        fedora|rhel|centos|rocky|almalinux)
            echo "For Fedora/RHEL-based systems, run:"
            echo ""
            echo "  sudo dnf install cmake qt6-qtbase-devel qt6-qtwebengine-devel"
            echo ""
            ;;
        arch|manjaro)
            echo "For Arch Linux-based systems, run:"
            echo ""
            echo "  sudo pacman -S cmake qt6-base qt6-webengine"
            echo ""
            ;;
        macos)
            echo "For macOS (using Homebrew), run:"
            echo ""
            echo "  brew install cmake qt@6"
            echo ""
            echo "After installation, you may need to set Qt6_DIR:"
            echo "  export Qt6_DIR=\$(brew --prefix qt@6)/lib/cmake/Qt6"
            echo ""
            ;;
        *)
            echo "For your system, please install:"
            echo "  - CMake 3.16 or higher"
            echo "  - Qt6 with WebEngineWidgets component"
            echo "  - A C++17 compatible compiler"
            echo ""
            ;;
    esac
}

# Function to offer automatic installation
offer_auto_install() {
    OS=$(detect_os)
    
    case "$OS" in
        ubuntu|debian|linuxmint)
            echo ""
            read -p "Would you like to attempt automatic installation? (y/N): " -n 1 -r
            echo ""
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                echo "Installing dependencies..."
                sudo apt-get update && sudo apt-get install -y cmake qt6-base-dev qt6-webengine-dev
                if [ $? -eq 0 ]; then
                    print_success "Dependencies installed successfully!"
                    return 0
                else
                    print_error "Automatic installation failed. Please install manually."
                    return 1
                fi
            fi
            ;;
        fedora|rhel|centos|rocky|almalinux)
            echo ""
            read -p "Would you like to attempt automatic installation? (y/N): " -n 1 -r
            echo ""
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                echo "Installing dependencies..."
                sudo dnf install -y cmake qt6-qtbase-devel qt6-qtwebengine-devel
                if [ $? -eq 0 ]; then
                    print_success "Dependencies installed successfully!"
                    return 0
                else
                    print_error "Automatic installation failed. Please install manually."
                    return 1
                fi
            fi
            ;;
        arch|manjaro)
            echo ""
            read -p "Would you like to attempt automatic installation? (y/N): " -n 1 -r
            echo ""
            if [[ $REPLY =~ ^[Yy]$ ]]; then
                echo "Installing dependencies..."
                sudo pacman -S --needed --noconfirm cmake qt6-base qt6-webengine
                if [ $? -eq 0 ]; then
                    print_success "Dependencies installed successfully!"
                    return 0
                else
                    print_error "Automatic installation failed. Please install manually."
                    return 1
                fi
            fi
            ;;
        macos)
            if command -v brew &> /dev/null; then
                echo ""
                read -p "Would you like to attempt automatic installation using Homebrew? (y/N): " -n 1 -r
                echo ""
                if [[ $REPLY =~ ^[Yy]$ ]]; then
                    echo "Installing dependencies..."
                    brew install cmake qt@6
                    if [ $? -eq 0 ]; then
                        print_success "Dependencies installed successfully!"
                        echo "Setting Qt6_DIR environment variable..."
                        export Qt6_DIR=$(brew --prefix qt@6)/lib/cmake/Qt6
                        return 0
                    else
                        print_error "Automatic installation failed. Please install manually."
                        return 1
                    fi
                fi
            else
                print_warning "Homebrew not found. Please install Homebrew first: https://brew.sh"
            fi
            ;;
    esac
    
    return 1
}

# Run dependency checks
echo "Checking build dependencies..."
echo ""

check_cmake || true
check_qt6 || true

echo ""

# If dependencies are missing, show instructions and exit
if [ $MISSING_DEPS -eq 1 ]; then
    print_install_instructions
    offer_auto_install
    
    if [ $? -eq 0 ]; then
        echo ""
        echo "Re-checking dependencies after installation..."
        echo ""
        MISSING_DEPS=0
        check_cmake || true
        check_qt6 || true
        echo ""
        
        if [ $MISSING_DEPS -eq 1 ]; then
            print_error "Some dependencies are still missing. Please check the installation and try again."
            exit 1
        fi
    else
        print_error "Please install the missing dependencies and try again."
        exit 1
    fi
fi

print_success "All dependencies are satisfied!"
echo ""

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
    echo "Creating build directory..."
    mkdir build
else
    echo "Build directory already exists."
fi

# Navigate to build directory
cd build

# Run CMake configuration
echo "Running CMake configuration..."
cmake ..

# Build the project
echo "Building the project..."
cmake --build .

echo ""
echo "Build complete!"
echo "Run the application with: ./SimpleBrowser (from the build directory)"
