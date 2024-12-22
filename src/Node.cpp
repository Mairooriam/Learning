#include "Node.hpp"
#include "Application.hpp"


void Node::updateSpritePosition(DTS::CameraController cController)
{

    sf::Vector2f newPos = DTS::worldPosToScreenPos(this->getPosition(), cController);
    sf::Vector2f oldPos = sprite.getPosition();
    sprite.setPosition(newPos);
}
