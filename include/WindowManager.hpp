#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <SFML/Graphics.hpp>

class WindowManager {
public:
    WindowManager();
    void run(sf::Sprite sprite);

private:
    void processEvents();
    void update();
    void render(sf::Sprite sprite);

    sf::RenderWindow window;
};

#endif // WINDOWMANAGER_HPP