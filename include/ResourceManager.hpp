#ifndef EXAMPLE_HPP
#define EXAMPLE_HPP
#include <SFML/Graphics.hpp>
#include <unordered_map>

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

    friend std::ostream& operator<<(std::ostream& os, const ResourceManager& rm);
private:
    
    void loadTextures(std::string resourceDir, std::unordered_map<std::string,sf::Texture>* target);
    void loadFonts(std::string resourceDir, std::unordered_map<std::string,sf::Font>* target);

    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    ResourceManager();

};



#endif // RESOURCEMANAGER_HPP


