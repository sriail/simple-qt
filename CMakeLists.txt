cmake_minimum_required(VERSION 3.16)
project(SimpleBrowser VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets WebEngineWidgets)

# Source files
set(SOURCES
    src/main.cpp
    src/browser.cpp
    src/webview.cpp
)

set(HEADERS
    src/browser.h
    src/webview.h
)

# Resources
set(RESOURCES
    resources.qrc
)

# Create executable
add_executable(SimpleBrowser
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# Link Qt libraries
target_link_libraries(SimpleBrowser
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineWidgets
)

# Installation
install(TARGETS SimpleBrowser
    RUNTIME DESTINATION bin
)
