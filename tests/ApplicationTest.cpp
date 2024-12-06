#include <gtest/gtest.h>
#include "Profiler.hpp"
#include "Application.hpp"

class HelperFunctionsTest : public ::testing::Test {
protected:

    void SetUp() override {
        // General setup code without SFML specifics
        testNode.position = {1.0f, 1.0f};
        testNode.spriteSize = {25.0f, 50.0f};
        cController.offset = {5.0f,5.0f};
        
    

    }
    DTS::Node testNode;
    DTS::CameraController cController;    
};

TEST_F(HelperFunctionsTest, worldPosToScreenPos) {
    // Overload 1: Using Node and CameraController
    DTS::Vector2f screenPos1 = DTS::worldPosToScreenPos(testNode, cController);
    EXPECT_EQ(screenPos1.x, 30.0f);
    EXPECT_EQ(screenPos1.y, 55.0f);

    // Overload 2: Using Vector2f, Size, and Vector2f
    DTS::Vector2f screenPos2 = DTS::worldPosToScreenPos({1.0f, 1.0f}, {25.0f, 50.0f}, {5.0f, 5.0f});
    EXPECT_EQ(screenPos2.x, 30.0f);
    EXPECT_EQ(screenPos2.y, 55.0f);
}

TEST_F(HelperFunctionsTest, screenPosToWorldPos) {
    // Overload 1: Using Node and CameraController
    DTS::Vector2f worldPos1 = DTS::screenPosToWorldPos({30.0f, 55.0f}, {25.0f, 50.0f}, {5.0f, 5.0f});
    EXPECT_EQ(worldPos1.x, 1.0f);
    EXPECT_EQ(worldPos1.y, 1.0f);

    // Overload 2: Using Vector2f, Size, and Vector2f
    DTS::Vector2f worldPos2 = DTS::screenPosToWorldPos({30.0f, 55.0f}, {25.0f, 50.0f}, {5.0f, 5.0f});
    EXPECT_EQ(worldPos2.x, 1.0f);
    EXPECT_EQ(worldPos2.y, 1.0f);
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}