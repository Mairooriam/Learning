#include "Application.hpp"

namespace DTS {
    

    sf::Vector2f worldPosToScreenPos(sf::Vector2i worldPosition, const DTS::CameraController cameraController) {
        sf::Vector2f screenPosition;
        screenPosition.x = worldPosition.x * (cameraController.cellSize.width + cameraController.spacing.x) + cameraController.offset.x;
        screenPosition.y = worldPosition.y * (cameraController.cellSize.height + cameraController.spacing.y) + cameraController.offset.y;
        return screenPosition; 
    }

    sf::Vector2i screenPosToWorldPos(const sf::Vector2i& screenPosition, const DTS::CameraController& cameraController) {
        sf::Vector2i worldPosition;
        worldPosition.x = (screenPosition.x + cameraController.offset.x) / (cameraController.cellSize.width + cameraController.spacing.x);
        worldPosition.y = (screenPosition.y + cameraController.offset.y) / (cameraController.cellSize.height + cameraController.spacing.y);
    return worldPosition;
    }





} // namespace DTS