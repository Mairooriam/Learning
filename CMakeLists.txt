cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Set the path to the local SFML directory
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-2.6.2)

# Add include directories
include_directories(${SFML_DIR}/include)

# Link directories
link_directories(${SFML_DIR}/lib)

# Add the main executable
add_executable(main src/main.cpp)
target_compile_definitions(main PRIVATE SFML_STATIC)
target_link_libraries(main PRIVATE sfml-graphics-s sfml-window-s sfml-system-s opengl32 freetype winmm gdi32 ws2_32)
target_include_directories(main PRIVATE ${SFML_DIR}/include)
target_compile_features(main PRIVATE cxx_std_17)

if(WIN32)
    add_custom_command(
        TARGET main
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_DIR}/bin/openal32.dll"
        $<TARGET_FILE_DIR:main>
        VERBATIM
    )
endif()
