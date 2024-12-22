#include <gtest/gtest.h>
#include "Profiler.hpp"
#include "Application.hpp"

class HelperFunctionsTest : public ::testing::Test {
protected:

    void SetUp() override {

        
    

    }

};

TEST_F(HelperFunctionsTest, worldPosToScreenPos) {

}

TEST_F(HelperFunctionsTest, screenPosToWorldPos) {

}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}