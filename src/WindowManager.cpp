#include "WindowManager.hpp"

WindowManager::WindowManager()
    : window(sf::VideoMode(800, 600), "SFML Window") {
}

void WindowManager::run(sf::Sprite sprite) {
    while (window.isOpen()) {
        processEvents();
        update();
        render(sprite);
    }
}

void WindowManager::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

void WindowManager::update() {
    // Update game logic here
}

void WindowManager::render(sf::Sprite sprite) {
    window.clear();
    window.draw(sprite);
    window.display();
}