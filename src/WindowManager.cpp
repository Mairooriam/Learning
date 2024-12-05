#include "WindowManager.hpp"
#include "Profiler.hpp"
#include "spdlog/spdlog.h"
#include "Benchmark.cpp"
WindowManager::WindowManager()
    : m_window(sf::VideoMode(800, 600), "SFML Window") {
    spdlog::flush_on(spdlog::level::info);
    spdlog::info("WindowManager::WindowManager: Window is opening!");
    
}

void WindowManager::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 Hz for rendering
    int i = 0;
    while (m_window.isOpen()) {
        sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        processEvents();

        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            update();
        }

        render();

        
    }
}
sf::RenderWindow &WindowManager::getWindow() {
    return m_window;
}


void WindowManager::updateSpriteArray(std::vector<sf::Sprite> &&spriteArray) {
    
    spdlog::info("WindowManager::updateSpriteArray: Updating spriteArray");
    m_spriteArray = std::move(spriteArray);
}
void WindowManager::processEvents() {
    
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        }
    }
}

void WindowManager::update() {
    // Update game logic here
}

void WindowManager::render() {
    m_window.clear();
    
    for (const auto& sprite : m_spriteArray) {
        m_window.draw(sprite);
        //spdlog::info("WindowManager::render Drawing sprites");
        
    }
    
    m_window.display();
}