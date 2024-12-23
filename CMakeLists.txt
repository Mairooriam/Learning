cmake_minimum_required(VERSION 3.28)
project(CMakeSDLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

## SPDLOG ##
include(FetchContent)

# Fetch spdlog
FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.9.2
)
FetchContent_MakeAvailable(spdlog)

## IMGUI ##
# Fetch ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG docking # Specify the branch name
)
FetchContent_MakeAvailable(imgui)

# Print the value of ${imgui_SOURCE_DIR}
message(STATUS "ImGui source directory: ${imgui_SOURCE_DIR}")

## SDL2 ##
# Fetch SDL2
FetchContent_Declare(
  sdl2
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG release-2.26.0
)
FetchContent_MakeAvailable(sdl2)

## INCLUDES ##
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src) # Include directory for SDLWindow.h
include_directories(${CMAKE_BINARY_DIR}/_deps/imgui-src) # Include ImGui headers
include_directories(${CMAKE_BINARY_DIR}/_deps/imgui-src/backends) # Include ImGui backends
include_directories(${CMAKE_BINARY_DIR}/_deps/sdl2-src/include) # Include SDL2 headers

# Create a static library for ImGui
add_library(imgui STATIC
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_draw.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_tables.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_widgets.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/imgui_demo.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_sdl2.cpp
    ${CMAKE_BINARY_DIR}/_deps/imgui-src/backends/imgui_impl_opengl3.cpp
)

# Add the main executable
add_executable(main
    src/main.cpp
    src/RenderWindow.cpp
)
find_package(OpenGL REQUIRED)
target_link_libraries(main PRIVATE spdlog::spdlog SDL2::SDL2 SDL2::SDL2main OpenGL::GL imgui)
target_include_directories(main PRIVATE ${CMAKE_BINARY_DIR}/_deps/imgui-src ${CMAKE_BINARY_DIR}/_deps/imgui-src/backends ${CMAKE_BINARY_DIR}/_deps/sdl2-src/include)
target_compile_features(main PRIVATE cxx_std_17)

# Custom command to copy resources folder
add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:main>/resources
)

# Custom target to trigger the command
add_custom_target(copy_resources ALL DEPENDS main)