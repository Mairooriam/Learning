cmake_minimum_required(VERSION 3.28)

set(This ExampleTests1)

set(Sources
    ExampleTests1.cpp
    Source.cpp
)

add_executable(${This} ${Sources})
target_link_libraries(${This} PUBLIC
    gtest_main
    CMakeSFMLProject
)
target_compile_definitions(${This} PRIVATE SFML_STATIC)

add_test(
    NAME ${This}
    COMMAND ${This}
)