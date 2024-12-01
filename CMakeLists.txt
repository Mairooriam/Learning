cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(This Example)

project(${This} C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Enable colored output
set(CMAKE_COLOR_MAKEFILE ON)

enable_testing()
# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
# Set the path to the local Google Test directory
set(GTEST_DIR ${CMAKE_SOURCE_DIR}/external/googletest-1.15.2)
add_subdirectory(${GTEST_DIR})

# SFML STUFF
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-2.6.2)
include_directories(${SFML_DIR}/include)
link_directories(${SFML_DIR}/lib)



set(Headers
    include/Example.hpp
    include/WindowManager.hpp
)
set(Sources
    src/Example.cpp
    src/WindowManager.cpp
)

add_library(${This} STATIC ${Sources} ${Header})

add_subdirectory(tests)