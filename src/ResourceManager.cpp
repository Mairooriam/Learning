#include "ResourceManager.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;





sf::IntRect ResourceManager::getTextureRect(size_t index)
{
    int x = index % config.textureMaxRows; // Column index
    int y = index / config.textureMaxRows; // Row index
    int size = config.textureSize;
    return sf::IntRect(x*size, y*size, size,size);
}

bool ResourceManager::isTextureValid(sf::Texture texture)
{
    return texture.getSize().x == config.textureResolution && texture.getSize().y == config.textureResolution;
}

sf::Texture &ResourceManager::getTexture(const std::string &fileName)
{
    auto it = textures.find(fileName);
    if (it == textures.end()) {
        spdlog::error("Texture not found: {}", fileName);
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

sf::Sprite ResourceManager::createSprite(const size_t index, std::string textureFileName)
{
    sf::Texture texture = getTexture(textureFileName);
    if (isTextureValid(texture)){
        sf::IntRect textureRect = getTextureRect(index);
        sf::Sprite sprite;
        sprite.setTexture(textures[textureFileName]);
        sprite.setTextureRect(textureRect);
        spdlog::debug("ResourceManager::createSprite: Created sprite {} from {}", textureFileName, index);
        return sprite;
    }else {
        spdlog::error("ResourceManager::createSprite: {} was not found in Textures", textureFileName);
        return sf::Sprite();
    }
}

ResourceManager::ResourceManager(const resourceManagerConfig& config) : config(config) {
    // Set the flush level to info
    spdlog::flush_on(spdlog::level::info);
    spdlog::info("ResourceManager initialized");

    loadTextures(config.texturesPath, &this->textures);
    loadFonts(config.fontsPath, &this->fonts);
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