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

struct ResourceManagerConfig
{
    int textureCellSize = 16;
    int textureResolution = 128;
    int textureMaxRows = textureResolution/textureCellSize;
    std::string texturesPath = "resources/textures";
    std::string fontsPath = "resources/fonts";

};  
class ResourceManager {
public:
    static ResourceManager& getInstance(const ResourceManagerConfig& config = ResourceManagerConfig()) {
        static ResourceManager instance(config);
        return instance;
    }
    void loadResources();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    sf::Sprite createSprite(const size_t index, sf::Texture& texture, ResourceManagerConfig config);
    sf::Sprite createSprite(const size_t index, sf::Texture& texture); // Overloaded function

    sf::Texture createCheckerboardTexture(ResourceManagerConfig config);
    sf::Font& getFont(const std::string& fileName);
    
    friend std::ostream& operator<<(std::ostream& os, const ResourceManager& rm);
    
    
    sf::Texture& getTexture(const std::string& fileName);
    ResourceManagerConfig getConfig() const {return this->config;}
    sf::Texture getErrorTexture()const {return this->m_errorTexture;}
private:
    ResourceManager(const ResourceManagerConfig& config);
    ResourceManager();
    
    sf::IntRect createTextureRect(const size_t index, ResourceManagerConfig config);
    bool isTextureValid(sf::Texture texture);
    void loadTextures(std::string resourceDir, std::unordered_map<std::string,sf::Texture>* target);
    void loadFonts(std::string resourceDir, std::unordered_map<std::string,sf::Font>* target);


    
    std::unordered_map<std::string, sf::Texture> textures;
    sf::Texture m_errorTexture;
    std::unordered_map<std::string, sf::Font> fonts;
    ResourceManagerConfig config;
};



#endif // RESOURCEMANAGER_HPP


