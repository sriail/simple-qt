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
    src/browserwindow.cpp
    src/tabwidget.cpp
    src/webpage.cpp
    src/webview.cpp
)

# Header files
set(HEADERS
    src/browser.h
    src/browserwindow.h
    src/tabwidget.h
    src/webpage.h
    src/webview.h
)

# Resources
set(RESOURCES
    resources/resources.qrc
)

# Create executable
add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
    ${RESOURCES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebEngineWidgets
)

# Set include directories
target_include_directories(${PROJECT_NAME} PRIVATE src)

# Install target
install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
)
