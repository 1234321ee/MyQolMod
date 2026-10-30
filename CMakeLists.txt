cmake_minimum_required(VERSION 3.21)
project(MyQoLMod)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include($ENV{GEODE_SDK}/cmake/Geode.cmake)

add_geode_mod(MyQoLMod
    SOURCES
        src/main.cpp
)