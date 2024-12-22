#include "WindowManager.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include "Profiler.hpp"
#include "Node.hpp"
#include "imgui.h"
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

    std::vector<Node> nodes;    
    

    {
        PROFILE_SCOPE("Create sprite test");
        struct GridConfig {
            int rows;
            int columns;
            float spacing; // Add spacing between tiles
        };

        


        GridConfig gridConfig = {5, 5, 5.0f}; // Adjust these values as needed
        int totalEls = gridConfig.columns*gridConfig.rows;
        nodes.reserve(totalEls); 
        sf::Texture& texture = resourceManager.getTexture("dsadasd.png");
       

        for (int row = 0; row < gridConfig.rows; ++row) {
            for (int col = 0; col < gridConfig.columns; ++col) {
            sf::Sprite sprite = resourceManager.createSprite(1, texture);
            sf::Vector2i pos = {row,col};
            nodes.emplace_back(pos, sprite);
            
            }
        }

    }
    WindowManager windowManager;
    windowManager.updateSpriteArray(std::move(nodes));
    
    windowManager.run();
  
    Instrumentor::Get().EndSession();
    
    return 0;
}
