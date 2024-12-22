#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Application.hpp"

#ifndef NODE_HPP
#define NODE_HPP



class Node {
public:
    Node(const sf::Vector2i& pos, const sf::Sprite& spr) : position(pos), sprite(spr) {}
    ~Node() = default; // Destructor
    sf::Vector2i getPosition() const { return position; }
    void setPosition(const sf::Vector2i& pos) { position = pos; }

    sf::Sprite getSprite() const { return sprite; }
    void setSprite(const sf::Sprite& spr) { sprite = spr; }

    void updateSpritePosition(DTS::CameraController cController);

    Node() = default; // Default constructor
    Node(const Node& other) = default; // Copy constructor
    Node(Node&& other) noexcept = default; // Move constructor
    Node& operator=(const Node& other) = default; // Copy assignment operator

    Node& operator=(Node&& other) noexcept {
        if (this != &other) {
            position = std::move(other.position);
            sprite = std::move(other.sprite);
        }
        return *this;
    }
private:
    sf::Vector2i position;
    sf::Sprite sprite;
};

#endif // NODE_HPP