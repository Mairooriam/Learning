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

struct resourceManagerConfig
{
    int textureSize = 16;
    int textureResolution = 128;
    int textureMaxRows = textureResolution/textureSize;
    std::string texturesPath = "resources/textures";
    std::string fontsPath = "resources/fonts";
    bool enableLogging = true;
};  
class ResourceManager {
public:
    static ResourceManager& getInstance(const resourceManagerConfig& config = resourceManagerConfig()) {
        static ResourceManager instance(config);
        return instance;
    }
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    sf::Sprite createSprite(const size_t index, std::string textureFileName);
    sf::Font& getFont(const std::string& fileName);
    sf::Texture& getTexture(const std::string& fileName);
    friend std::ostream& operator<<(std::ostream& os, const ResourceManager& rm);
private:
    ResourceManager(const resourceManagerConfig& config);
    ResourceManager();

    sf::IntRect getTextureRect(const size_t index);
    bool isTextureValid(sf::Texture texture);
    void loadTextures(std::string resourceDir, std::unordered_map<std::string,sf::Texture>* target);
    void loadFonts(std::string resourceDir, std::unordered_map<std::string,sf::Font>* target);


    
    std::unordered_map<std::string, sf::Texture> textures;
    std::unordered_map<std::string, sf::Font> fonts;
    resourceManagerConfig config;
};



#endif // RESOURCEMANAGER_HPP


