cmake_minimum_required(VERSION 3.16)
project(ExpenseTracker VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

# Works with either Qt5 or Qt6 - CMake will pick whichever is found first.
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(ExpenseTracker
    main.cpp
    mainwindow.cpp
    mainwindow.h
)

target_link_libraries(ExpenseTracker PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
