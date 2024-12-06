#include "Application.hpp"

namespace DTS {
    

Vector2f worldPosToScreenPos(Vector2f worldPosition, Size cellSize, Vector2f offset){
    Vector2f screenPosition;
    screenPosition.x = worldPosition.x * cellSize.width + offset.x;
    screenPosition.y = worldPosition.y * cellSize.height + offset.y;
    return screenPosition;    
}

Vector2f worldPosToScreenPos(Node node, CameraController cameraController) {
    return worldPosToScreenPos(node.position, node.spriteSize, cameraController.offset);
}

Vector2f screenPosToWorldPos(Vector2f screenPosition, Size cellSize, Vector2f offset){
    Vector2f worldPosition;
    worldPosition.x = (screenPosition.x - offset.x) / cellSize.width;
    worldPosition.y = (screenPosition.y - offset.y) / cellSize.height;
    return worldPosition;
}
Vector2f screenPosToWorldPos(Node node, CameraController cameraController) {
    return screenPosToWorldPos(node.position, node.spriteSize, cameraController.offset);
}
} // namespace DTS