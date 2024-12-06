#ifndef APPLICATION_HPP
#define APPLICATION_HPP

namespace DTS {

/**
 * @brief Represents a 2D vector with x and y coordinates.
 */
struct Vector2f {
    float x;
    float y;
};

/**
 * @brief Represents the size with width and height.
 */
struct Size {
    float width;
    float height;
};

/**
 * @brief Represents a node with a position and sprite size.
 */
struct Node {
    Vector2f position;
    Size spriteSize;
};

/**
 * @brief Represents the camera controller with an offset.
 */
class CameraController {
public:
    Vector2f offset;
};

/**
 * @brief Converts world position to screen position.
 * 
 * @param worldPosition The world position.
 * @param size The size of the sprite.
 * @param offset The offset of the camera.
 * @return The screen position.
 */
Vector2f worldPosToScreenPos(Vector2f worldPosition, Size cellSize, Vector2f offset);

/**
 * @brief Converts world position to screen position using a Node and CameraController.
 * 
 * @param node The node containing the position and sprite size.
 * @param cameraController The camera controller containing the offset.
 * @return The screen position.
 */
Vector2f worldPosToScreenPos(Node node, CameraController cameraController);
/**
 * @brief Converts a screen position to a world position.
 * 
 * @param worldPosition The world position.
 * @param cellSize The size of the cell.
 * @param offset The offset of the camera.
 * @return The world position (px).
 */
Vector2f screenPosToWorldPos(Vector2f screenPosition, Size cellSize, Vector2f offset);
/**
 * @brief Converts a screen position to a world position.
 * 
 * This function takes a screen position and converts it to the corresponding
 * world position based on the provided node and camera controller.
 * 
 * @param node The node in the scene graph to which the screen position is relative.
 * @param cameraController The camera controller that provides the view and projection matrices.
 * @return Vector2f The world position corresponding to the given screen position.
 */
Vector2f screenPosToWorldPos(Node node, CameraController cameraController);
} // namespace DTS

#endif // APPLICATION_HPP