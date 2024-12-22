#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <SFML/Graphics.hpp>
#include "Application.hpp"
#include "Node.hpp"
struct MouseState {
    sf::Vector2i position; 
    bool isClicked;        
    bool isActive;         
};


class WindowManager {
public:
    WindowManager();
    void run();
    sf::RenderWindow& getWindow();
    void updateSpriteArray(std::vector<Node> &&spriteArray);
private:
    void processEvents();
    void updateSpritesPosition();
    void handleMouseEvent(const sf::Event& event);
    void handleKeyEvent(const sf::Event& event);
    void initCameraController();
    void update();
    void render();
    MouseState m_mouse;
    sf::RenderWindow m_window;
    sf::View m_view;
    DTS::CameraController cController;
    std::vector<Node> m_spriteArray;
};;

#endif // WINDOWMANAGER_HPP