#include "WindowManager.hpp"
#include "ResourceManager.hpp"
#include <iostream>

int main()
{
    ResourceManager& resourceManager = ResourceManager::getInstance();
    
    std::cout << resourceManager << "\n";

    WindowManager windowManager;
    windowManager.run();
    
    return 0;
}
