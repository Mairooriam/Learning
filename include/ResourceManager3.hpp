#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <string>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "resourceManager.hpp"
#include <iostream>
#include <stdexcept>

enum WallTextureType{
    WALL_VERTICAL = 0,
    WALL_HORIZONTAL,
    WALL_JUNCTION,
    WALL_CORNER_BOTTOM_LEFT,
    WALL_CORNER_BOTTOM_RIGHT,
    WALL_CORNER_TOP_LEFT,
    WALL_CORNER_TOP_RIGHT,
    WALL_JUNCTION_TOP_LEFT_BOTTOM,
    WALL_JUNCTION_TOP_RIGHT_BOTTOM,
    WALL_JUNCTION_LEFT_TOP_RIGHT,
    WALL_JUNCTION_LEFT_BOTTOM_RIGHT,
    WALL_LONELY,
    FLOOR_GREEN
};

    
class ResourceManager {
public:
    static ResourceManager& getInstance() {
        static ResourceManager instance;
        return instance;
    }

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Creates sprite of supplied texture and index
    sf::Sprite createSprite16x16(const std::string& textureName, const WallTextureType texture);
    sf::IntRect getTextureRect(const int index, const int size, const int rows, const int columns);
    // Retrieve a font by name
    sf::Font& getFont(const std::string& name);
private:
    ResourceManager();
    // returns intRect according to index and texture dimensions.
    sf::IntRect getTextureRectWall(const WallTextureType textureEnum, const int size, const int rows, const int columns);
    // Retrieve a texture by name
    sf::Texture& getTexture(const std::string& name);
    
    // Load a texture from a file and store it with a given name
    void loadTexture(const std::string& name, const std::string& filename);
    // Load a Font from a file and store it with given name
    void loadFont(const std::string& name, const std::string& filename);
    // Map to store textures by name
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    std::string resourcePath = "resources/";
};

#endif // RESOURCEMANAGER_HPP