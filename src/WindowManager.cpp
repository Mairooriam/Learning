#include "WindowManager.hpp"
#include "Profiler.hpp"
#include "spdlog/spdlog.h"
#include "Application.hpp"



WindowManager::WindowManager()
    : m_window(sf::VideoMode(800, 600), "SFML Window") {
    spdlog::set_level(spdlog::level::info);
    spdlog::flush_on(spdlog::level::info);
    spdlog::info("WindowManager::WindowManager: Window is opening!");
    m_view = m_window.getDefaultView();
    initCameraController();
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


void WindowManager::updateSpriteArray(std::vector<Node> &&spriteArray) {
    
    spdlog::info("WindowManager::updateSpriteArray: Updating spriteArray");
    m_spriteArray = std::move(spriteArray);
}
void WindowManager::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            m_window.close();
        } else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased || event.type == sf::Event::MouseMoved) {
            handleMouseEvent(event);
        } else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {
            handleKeyEvent(event);
        } else if (event.type == sf::Event::Resized) {
                m_view.setSize(event.size.width, event.size.height);
                m_window.setView(m_view);
            }
    }
}

void WindowManager::updateSpritesPosition()
{
    for (size_t i = 0; i < m_spriteArray.size(); i++)
    {
        m_spriteArray[i].updateSpritePosition(cController);
    }
    
}

void WindowManager::handleMouseEvent(const sf::Event& event) {
    m_mouse.position = sf::Mouse::getPosition(m_window);
    sf::Vector2i worldPos = DTS::screenPosToWorldPos(m_mouse.position, cController);
    switch (event.type) {
    case sf::Event::MouseButtonPressed:
        if (event.mouseButton.button == sf::Mouse::Left) {
            updateSpritesPosition();
            spdlog::info("Mouse button 1 (Left) pressed at ({}, {}) World: ({}, {})", event.mouseButton.x, event.mouseButton.y, worldPos.x, worldPos.y);
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            updateSpritesPosition();
            spdlog::info("Mouse button 2 (Right) pressed at ({}, {}) World: ({}, {})", event.mouseButton.x, event.mouseButton.y, worldPos.x, worldPos.y);
        } else if (event.mouseButton.button == sf::Mouse::Middle) {
            spdlog::info("Mouse button 3 (Middle) pressed at ({}, {}) World: ({}, {})", event.mouseButton.x, event.mouseButton.y, worldPos.x, worldPos.y);
        }
        
        break;
    case sf::Event::MouseButtonReleased:
        if (event.mouseButton.button == sf::Mouse::Left) {
            spdlog::info("Mouse button 1 (Left) released at ({}, {})", event.mouseButton.x, event.mouseButton.y);
        } else if (event.mouseButton.button == sf::Mouse::Right) {
            spdlog::info("Mouse button 2 (Right) released at ({}, {})", event.mouseButton.x, event.mouseButton.y);
        } else if (event.mouseButton.button == sf::Mouse::Middle) {
            spdlog::info("Mouse button 3 (Middle) released at ({}, {})", event.mouseButton.x, event.mouseButton.y);
        }
        break;
    case sf::Event::MouseMoved:
        // spdlog::info("Mouse moved to ({}, {})", event.mouseMove.x, event.mouseMove.y);
        break;
    default:
        break;
    }
}


void WindowManager::handleKeyEvent(const sf::Event& event) {
    // Handle key events here
}

void WindowManager::initCameraController(){
    cController.offset = {0.0,0.0};
    cController.cellSize.height = 16.0f;
    cController.cellSize.width = 16.0f;
    cController.spacing = {5,5};
}

void WindowManager::update() {
    updateSpritesPosition();
}

void WindowManager::render() {
    m_window.clear();
    
    for (const auto& node : m_spriteArray) {
        m_window.draw(node.getSprite());
        //spdlog::info("WindowManager::render Drawing sprites");
        
    }
    
    m_window.display();
}