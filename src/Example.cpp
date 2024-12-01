// src/example.cpp
#include "example.hpp"
#include "WindowManager.hpp"


int add(int a, int b) {
    return a + b;
}

bool isEven(int number) {
    return number % 2 == 0;
}

WindowManager windowManager;

int main() {
    windowManager.run();
    return 0;
}