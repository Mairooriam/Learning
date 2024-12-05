#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <SFML/Graphics.hpp>

class WindowManager {
public:
    WindowManager();
    void run();
    sf::RenderWindow& getWindow();
    void updateSpriteArray(std::vector<sf::Sprite> &&spriteArray);
private:
    void processEvents();
    void update();
    void render();
    
    sf::RenderWindow m_window;
    std::vector<sf::Sprite> m_spriteArray;
};;

#endif // WINDOWMANAGER_HPP