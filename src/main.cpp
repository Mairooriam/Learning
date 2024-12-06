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
    
        //     sprite.scale(5.0f, 5.0f);
        
        // // Set the position with an offset
        // float xOffset = 10.0f; // Adjust this value as needed
        // float yOffset = 10.0f; // Adjust this value as needed
        // sprite.setPosition(i * xOffset, i * yOffset);
    std::vector<sf::Sprite> sprites;    
    sprites.reserve(1000); // Pre-allocate memory for 1000 elements

    {
        PROFILE_SCOPE("Create sprite test");
        struct GridConfig {
            int rows;
            int columns;
            float spacing; // Add spacing between tiles
        };

        GridConfig gridConfig = {100, 100, 5.0f}; // Adjust these values as needed

        sf::Texture& texture = resourceManager.getTexture("dsadasd.png");
       

        for (int row = 0; row < gridConfig.rows; ++row) {
            for (int col = 0; col < gridConfig.columns; ++col) {
            sf::Sprite sprite = resourceManager.createSprite(1, texture);
            sf::FloatRect spriteSize = sprite.getLocalBounds();
            sprite.setPosition(col * (spriteSize.height + gridConfig.spacing), row * (spriteSize.width + gridConfig.spacing));
            sprites.push_back(sprite);
            
            }
        }

    }
    WindowManager windowManager;
    windowManager.updateSpriteArray(std::move(sprites));
    
    windowManager.run();
  
    Instrumentor::Get().EndSession();
    
    return 0;
}
