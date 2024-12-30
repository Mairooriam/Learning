cmake_minimum_required(VERSION 3.28)
project(CMakeSDLProject LANGUAGES CXX)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)

# Use the multi-threaded DLL runtime library
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

# Set the output directory for binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Define common compile definitions
add_compile_definitions(MIR_PLATFORM_WINDOWS)


# Add subdirectories
add_subdirectory(Mir)
add_subdirectory(sandbox)