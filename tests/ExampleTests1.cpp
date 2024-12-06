#include <gtest/gtest.h>
#include "ResourceManager.hpp"
#include "WindowManager.hpp"
#include "Profiler.hpp"

class ResourceManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up the resource manager with a test configuration
        test_config.textureCellSize = 16;
        test_config.textureResolution = 128;
        test_config.texturesPath = "resources/textures";
        test_config.fontsPath = "resources/fonts";

        resourceManager = &ResourceManager::getInstance(test_config);
    }

    ResourceManagerConfig test_config;
    ResourceManager* resourceManager;
};

// Test ResourceManager initialization
TEST_F(ResourceManagerTest, Initialization) {
    EXPECT_EQ(resourceManager->getConfig().textureCellSize, 16);
    EXPECT_EQ(resourceManager->getConfig().textureResolution, 128);
    EXPECT_EQ(resourceManager->getConfig().texturesPath, "resources/textures");
    EXPECT_EQ(resourceManager->getConfig().fontsPath, "resources/fonts");
}

// Test checkerboard texture creation
TEST_F(ResourceManagerTest, CreateCheckerboardTexture) {
    sf::Texture texture = resourceManager->createCheckerboardTexture(test_config);
    sf::Image image = texture.copyToImage();

    // Check some pixels to ensure the checkerboard pattern
    EXPECT_EQ(image.getPixel(0, 0), sf::Color::Magenta);
    EXPECT_EQ(image.getPixel(1, 0), sf::Color::Black);
    EXPECT_EQ(image.getPixel(0, 1), sf::Color::Black);
    EXPECT_EQ(image.getPixel(1, 1), sf::Color::Magenta);
    EXPECT_EQ(texture.getSize().x, 128);
    EXPECT_EQ(texture.getSize().y, 128);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}