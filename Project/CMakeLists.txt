cmake_minimum_required(VERSION 3.26)
project(Project)

set(CMAKE_CXX_STANDARD 23)

add_executable(Project
        plane.cpp
        plane.h
        flight.h
        flight.cpp
        menu.h
        menu.cpp
        runway.h
        runway.cpp
)
