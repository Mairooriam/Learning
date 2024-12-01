cmake_minimum_required(VERSION 3.28)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS ${CMAKE_BINARY_DIR}/tests)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

## SFML STUFF ##
# Set the path to the local SFML directory
set(SFML_DIR ${CMAKE_SOURCE_DIR}/external/SFML-2.6.2)

## INCLUDES ##
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${SFML_DIR}/include)

# Link directories
link_directories(${SFML_DIR}/lib)

## GOOGLE TEST STUFF ##
# Set the path to the local googletest
set(GTEST_DIR ${CMAKE_SOURCE_DIR}/external/googletest-1.15.2)
add_subdirectory(${GTEST_DIR})

# Add the main executable
add_executable(main 
src/main.cpp
src/example.cpp
src/WindowManager.cpp
)
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

##TESTING##
# Enable testing
enable_testing()
set(CTEST_USE_LAUNCHERS OFF)
# Add the test executable
add_executable(ExampleTests tests/ExampleTests.cpp src/example.cpp)
set_target_properties(ExampleTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
target_compile_definitions(ExampleTests PRIVATE SFML_STATIC)
target_link_libraries(ExampleTests PRIVATE gtest_main sfml-graphics-s sfml-window-s sfml-system-s opengl32 freetype winmm gdi32 ws2_32)
target_include_directories(ExampleTests PRIVATE ${SFML_DIR}/include ${CMAKE_SOURCE_DIR}/include)
target_compile_features(ExampleTests PRIVATE cxx_std_17)

# Register the test
add_test(NAME ExampleTests COMMAND ExampleTests)