cmake_minimum_required(VERSION 3.16)

project(QtHello)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(QtHello
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

target_link_libraries(QtHello
    Qt5::Widgets
)