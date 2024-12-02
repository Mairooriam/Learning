#include "WindowManager.hpp"
#include "ResourceManager.hpp"
#include <iostream>

int main()
{
    ResourceManager& resourceManager = ResourceManager::getInstance();
    
    std::cout << resourceManager << "\n";

    sf::Sprite sprite1 =  resourceManager.createSprite(1,resourceManager.getTexture("wall_texturfffes.png"));
    sprite1.scale(50.0f,50.0f);
    WindowManager windowManager;
    windowManager.run(sprite1);
    
    
    return 0;
}
