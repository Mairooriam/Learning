#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "spdlog/spdlog.h"

namespace DTS {




    struct Size {
        float width;
        float height;
    };

    class CameraController {
    public:
        sf::Vector2f offset;
        Size cellSize; // Add cellSize to CameraController
        sf::Vector2f spacing;
    };


    sf::Vector2f worldPosToScreenPos(sf::Vector2i worldPosition, const CameraController cameraController);
    sf::Vector2i screenPosToWorldPos(const sf::Vector2i& screenPosition, const CameraController& cameraController);


} // namespace DTS

#endif // APPLICATION_HPP