#include "CollisionEngine.h"

bool CollisionEngine::checkCollision(const SpaceShipLogic& ship, const MeteorManager& meteors) const {
    int shipX = ship.getX();
    int shipY = ship.getY();

    for (const auto& m : meteors.getMeteors()) {
        if (m.y == shipY - 1 && m.x > shipX && m.x < shipX + 7) {
            return true;
        }
    }
    return false;
}
