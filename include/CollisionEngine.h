#pragma once
#include "SpaceShipLogic.h"
#include "MeteorManager.h"

class CollisionEngine {
public:
    CollisionEngine() = default;
    ~CollisionEngine() = default;
    bool checkCollision(const SpaceShipLogic& ship, const MeteorManager& meteors) const;
};
