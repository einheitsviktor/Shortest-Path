cmake_minimum_required(VERSION 3.5)

project(Shortest-Path VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Concurrent REQUIRED)


include_directories(include)
set(PROJECT_SOURCES
        src/main.cpp
        src/visualizer.cpp
        src/grid.cpp
        src/visualizer.ui

        include/helper.h
        include/visualizer.h
        include/grid.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Shortest-Path
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    if(ANDROID)
        add_library(Shortest-Path SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Shortest-Path
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Shortest-Path PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Shortest-Path PRIVATE Qt5::Concurrent)


set_target_properties(Shortest-Path PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Shortest-Path)
endif()

# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
