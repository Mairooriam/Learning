#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include <SFML/Graphics.hpp>

class WindowManager {
public:
    WindowManager();
    void run();

private:
    void processEvents();
    void update();
    void render();

    sf::RenderWindow window;
};

#endif // WINDOWMANAGER_HPP