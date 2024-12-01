#include "ResourceManager.hpp"
#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;
ResourceManager::ResourceManager() {
    // Set the flush level to info
    spdlog::flush_on(spdlog::level::info);
    spdlog::info("ResourceManager initialized");

    loadTextures("resources/textures", &this->textures);
    loadFonts("resources/fonts", &this->fonts);
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
    // TEXTURES
    os << "Textures loaded: ";
    for (const auto& pair : rm.textures) {
        os << "["<<pair.first << "]";
    }

    // FONTS
    os << "\nFonts loaded: ";
    for (const auto& pair : rm.fonts) {
        os << "["<<pair.first << "]";
    }
    return os;
}