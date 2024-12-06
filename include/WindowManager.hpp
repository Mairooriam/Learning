#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <SFML/Graphics.hpp>
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
    void updateSpriteArray(std::vector<sf::Sprite> &&spriteArray);
private:
    void processEvents();
    void handleMouseEvent(const sf::Event& event);
    void handleKeyEvent(const sf::Event& event);
    void update();
    void render();
    MouseState m_mouse;
    sf::RenderWindow m_window;
    std::vector<sf::Sprite> m_spriteArray;
};;

#endif // WINDOWMANAGER_HPP