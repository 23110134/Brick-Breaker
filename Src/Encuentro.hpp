#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <limits>
#include <set>
#include <string>

// Function to calculate which side the collision occurred on
std::string collisionHelperAABB(const sf::FloatRect& stationary, const sf::FloatRect& moving, const sf::Vector2f& prevPos, const sf::Vector2f& velocity) {
    float cornerSlopeRise = 0, cornerSlopeRun = 0;
    float velocitySlope = (velocity.x != 0) ? velocity.y / velocity.x : ((velocity.y > 0) ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity());
    std::set<std::string> potentialCollisionSide;

    if (prevPos.x + moving.width <= stationary.left) {
        potentialCollisionSide.insert("left");
        cornerSlopeRun = stationary.left - (prevPos.x + moving.width);

        if (prevPos.y + moving.height <= stationary.top) {
            potentialCollisionSide.insert("top");
            cornerSlopeRise = stationary.top - (prevPos.y + moving.height);
        } else if (prevPos.y >= stationary.top + stationary.height) {
            potentialCollisionSide.insert("bottom");
            cornerSlopeRise = stationary.top + stationary.height - prevPos.y;
        } else {
            return "left";
        }
    } else if (prevPos.x >= stationary.left + stationary.width) {
        potentialCollisionSide.insert("right");
        cornerSlopeRun = prevPos.x - (stationary.left + stationary.width);

        if (prevPos.y + moving.height <= stationary.top) {
            potentialCollisionSide.insert("top");
            cornerSlopeRise = stationary.top - (prevPos.y + moving.height);
        } else if (prevPos.y >= stationary.top + stationary.height) {
            potentialCollisionSide.insert("bottom");
            cornerSlopeRise = prevPos.y - (stationary.top + stationary.height);
        } else {
            return "right";
        }
    } else {
        if (prevPos.y + moving.height <= stationary.top) {
            return "top";
        } else if (prevPos.y >= stationary.top + stationary.height) {
            return "bottom";
        }
    }

    if (cornerSlopeRun == 0) cornerSlopeRun = 0.000001f;

    // Determine the side from the slope
    float cornerSlope = cornerSlopeRise / cornerSlopeRun;
    if (potentialCollisionSide.count("top")) {
        if (potentialCollisionSide.count("left")) {
            return (velocitySlope < cornerSlope) ? "top" : "left";
        } else if (potentialCollisionSide.count("right")) {
            return (velocitySlope > cornerSlope) ? "top" : "right";
        }
    } else if (potentialCollisionSide.count("bottom")) {
        if (potentialCollisionSide.count("left")) {
            return (velocitySlope > cornerSlope) ? "bottom" : "left";
        } else if (potentialCollisionSide.count("right")) {
            return (velocitySlope < cornerSlope) ? "bottom" : "right";
        }
    }

    return "corner";
}

// Function to deflect a ball
sf::Vector2f controlledDeflect(const sf::FloatRect& ballRect, sf::Vector2f ballVelocity, const sf::FloatRect& playerRect) {
    float ballSpeed = std::sqrt(ballVelocity.x * ballVelocity.x + ballVelocity.y * ballVelocity.y);
    float distanceFromCenter = ballRect.left + ballRect.width / 2 - (playerRect.left + playerRect.width / 2);
    float maxCollisionPoint = playerRect.width / 2 + ballRect.width / 2;
    float percentFromMax = distanceFromCenter / maxCollisionPoint;
    float dx = std::min(ballSpeed * percentFromMax, ballSpeed * 0.8f);
    float dy = -std::sqrt(ballSpeed * ballSpeed - dx * dx);
    return sf::Vector2f(dx, dy);
}

int main() {
    // Test the collision function
    sf::FloatRect stationary(100, 100, 50, 50);
    sf::FloatRect moving(90, 90, 20, 20);
    sf::Vector2f previousPosition(80, 80);
    sf::Vector2f velocity(10, 10);
    std::string collisionSide = collisionHelperAABB(stationary, moving, previousPosition, velocity);
    std::cout << "Collision side: " << collisionSide << std::endl;

    // Test controlled deflection
    sf::Vector2f newVelocity = controlledDeflect(moving, velocity, stationary);
    std::cout << "New velocity: (" << newVelocity.x << ", " << newVelocity.y << ")" << std::endl;

    return 0;
}