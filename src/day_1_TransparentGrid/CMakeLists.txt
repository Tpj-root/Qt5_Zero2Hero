cmake_minimum_required(VERSION 3.16)

project(TransparentGrid)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 REQUIRED COMPONENTS Widgets)

add_executable(app
    main.cpp
    GridWidget.cpp
    GridWidget.h
)

target_link_libraries(app
    Qt5::Widgets
)