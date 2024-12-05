#include "WindowManager.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include "Profiler.hpp"
int main()
{
    ResourceManager& resourceManager = ResourceManager::getInstance();
    resourceManager.loadResources();
    std::cout << resourceManager << "\n";
    Instrumentor::Get().BeginSession("Profile");
    std::vector<sf::Sprite> sprites;
    

    for (int i = 0; i < 1000; ++i) {
        sf::Sprite sprite = resourceManager.createSprite(i, resourceManager.getTexture("dsadasd.png"));
        sprite.scale(5.0f, 5.0f);
        
        // Set the position with an offset
        float xOffset = 10.0f; // Adjust this value as needed
        float yOffset = 10.0f; // Adjust this value as needed
        sprite.setPosition(i * xOffset, i * yOffset);
        
        sprites.push_back(sprite);
    }
    
    
    WindowManager windowManager;
    windowManager.updateSpriteArray(std::move(sprites));
    windowManager.run();
    
    Instrumentor::Get().EndSession();
    
    return 0;
}
