#include "ResourceManager.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;





sf::IntRect ResourceManager::createTextureRect(size_t index, ResourceManagerConfig config)
{
    int x = index % config.textureMaxRows; // Column index
    int y = index / config.textureMaxRows; // Row index
    int size = config.textureSize;
    spdlog::info("ResourceManager::createTextureRect: Created textureRect x:{} y:{} size:{}",x,y,size);
    return sf::IntRect(x*size, y*size, size,size);
}

bool ResourceManager::isTextureValid(sf::Texture texture)
{
    spdlog::info("ResourceManager::isTextureValid: Texture size x:{} y:{}", texture.getSize().x, texture.getSize().y);
    return texture.getSize().x == config.textureResolution && texture.getSize().y == config.textureResolution;
}

sf::Texture &ResourceManager::getTexture(const std::string &fileName)
{
    auto it = textures.find(fileName);
    if (it == textures.end()) {
        spdlog::error("ResourceManager::getTexture Texture not found: {}", fileName);
        static sf::Texture emptyTexture;
        return emptyTexture;
    }
    return it->second;
}

sf::Font &ResourceManager::getFont(const std::string &fileName)
{
    auto it = fonts.find(fileName);
    if (it == fonts.end()) {
        spdlog::error("Font not found: {}", fileName);
        static sf::Font emptyFont;
        return emptyFont;
    }
    return it->second;
}

sf::Sprite ResourceManager::createSprite(const size_t index, sf::Texture& texture, ResourceManagerConfig config)
{
    if (isTextureValid(texture)){
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(createTextureRect(index, config));
        spdlog::debug("ResourceManager::createSprite: Created sprite from index {}", index);
        return sprite;
    } else {
        spdlog::error("ResourceManager::createSprite: texture provided is not valid... Using errorTexture ");
        sf::Sprite sprite;
        sprite.setTexture(errorTexture);
        sprite.setTextureRect(createTextureRect(index, config));
        return sprite;
    }
}

sf::Sprite ResourceManager::createSprite(const size_t index, sf::Texture& texture)
{
    return createSprite(index, texture, this->config);
}
sf::Texture ResourceManager::createCheckerboardTexture(ResourceManagerConfig config)
{
    const int size = config.textureResolution;
    sf::Image image;
    image.create(size, size, sf::Color::Black); // Initialize the image with black color

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            sf::Color color = ((x + y) % 2 == 0) ? sf::Color::Magenta : sf::Color::Black;
            image.setPixel(x, y, color);
        }
    }

    sf::Texture texture;
    if (!texture.loadFromImage(image)) {
        spdlog::error("ResourceManager::createCheckerboardTexture: Failed to load texture from image");
    }
    spdlog::warn("ResourceManager::createCheckerboardTexture: Error texture might be visible somewhere!");
    return texture;
}
ResourceManager::ResourceManager(const ResourceManagerConfig& config) : config(config) {
    // Set the flush level to info
    spdlog::flush_on(spdlog::level::info);
    spdlog::info("ResourceManager initialized");

    loadTextures(config.texturesPath, &this->textures);
    loadFonts(config.fontsPath, &this->fonts);
    errorTexture = createCheckerboardTexture(this->config);
}




void ResourceManager::loadTextures(std::string resourceDir, std::unordered_map<std::string, sf::Texture>* target) {
    for (const auto& entry : fs::directory_iterator(resourceDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            std::string filename = entry.path().filename().string();
            sf::Texture texture;
            if (!texture.loadFromFile(entry.path().string())) {
                spdlog::error("Failed to load texture: {}", filename);
            }
            (*target)[filename] = std::move(texture);
            spdlog::info("Loaded texture: {}", filename);
        }
    }
}

void ResourceManager::loadFonts(std::string resourceDir, std::unordered_map<std::string,sf::Font>* target){
    for (const auto& entry : fs::directory_iterator(resourceDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".TTF") {
            std::string filename = entry.path().filename().string();
            sf::Font font;
            if (!font.loadFromFile(entry.path().string())) {
                spdlog::error("Failed to load Font: {}", filename);
            }
            (*target)[filename] = std::move(font);
            spdlog::info("Loaded Font: {}", filename);
        }
    }
}

std::ostream& operator<<(std::ostream& os, const ResourceManager& rm) {
    // CONFIG
    os << "############# RESOURCE MANAGER PRINT #############\n";
    os << "  ########### ResourceManager config ###########\n";
    os << std::setw(20) << std::left << "Texture size:" << rm.config.textureSize << "\n";
    os << std::setw(20) << std::left << "Texture area size:" << rm.config.textureResolution << "\n";
    os << std::setw(20) << std::left << "Texture max row:" << rm.config.textureMaxRows << "\n";
    os << std::setw(20) << std::left << "Textures path:" << rm.config.texturesPath << "\n";
    os << std::setw(20) << std::left << "Fonts path:" << rm.config.fontsPath << "\n";
    os << std::setw(20) << std::left << "Logging enabled:" << std::boolalpha << rm.config.enableLogging << "\n\n";

    // TEXTURES
    os << "  ########### Textures loaded ###########\n";
    for (const auto& pair : rm.textures) {
        os << std::setw(20) << std::left << pair.first << "\n";
    }

    // FONTS
    os << "\n  ########### Fonts loaded ###########\n";
    for (const auto& pair : rm.fonts) {
        os << std::setw(20) << std::left << pair.first << "\n";
    }
    return os;
}