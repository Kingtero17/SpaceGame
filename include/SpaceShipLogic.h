#pragma once
#include "Renderer.h"

class SpaceShipLogic {
public:
    SpaceShipLogic();
    ~SpaceShipLogic() = default;

    void reset();
    void moveLeft();
    void moveRight();
    void moveUp();
    void loseHeart();
    void loseLife();
    void resetHearts();

    [[nodiscard]] int getX() const;
    [[nodiscard]] int getY() const;
    [[nodiscard]] int getLives() const;
    [[nodiscard]] int getHearts() const;

    void draw(Renderer& renderer, int stage = 0) const;

private:
    int ix_;
    int iy_;
    int lives_;
    int hearts_;

    const int minX_ = 4;
    const int maxX_ = 111;
};
