#include "SpaceShipLogic.h"

// Sprites Nave
static const std::string ship_row1 = "   *   ";
static const std::string ship_row2 = " |***| ";
static const std::string ship_row3 = "** * **";

// Explosion Nave #1
static const std::string exp1_row1 = "  **   ";
static const std::string exp1_row2 = " ****  ";
static const std::string exp1_row3 = "  **   ";

// Explosion Nave #2
static const std::string exp2_row1 = "* ** * ";
static const std::string exp2_row2 = " ****  ";
static const std::string exp2_row3 = "* ** * ";

SpaceShipLogic::SpaceShipLogic() {
    reset();
}

void SpaceShipLogic::reset() {
    ix_ = 54;
    iy_ = 19;
    lives_ = 3;
    hearts_ = 3;
}

void SpaceShipLogic::moveLeft() {
    if (ix_ > minX_) {
        ix_ -= 2;
    }
}

void SpaceShipLogic::moveRight() {
    if (ix_ < maxX_) {
        ix_ += 2;
    }
}

void SpaceShipLogic::moveUp() {
    iy_ -= 1;
}

void SpaceShipLogic::loseHeart() {
    if (hearts_ > 0) {
        hearts_--;
    }
}

void SpaceShipLogic::loseLife() {
    if (lives_ > 0) {
        lives_--;
    }
}

void SpaceShipLogic::resetHearts() {
    hearts_ = 3;
}

int SpaceShipLogic::getX() const {
    return ix_;
}

int SpaceShipLogic::getY() const {
    return iy_;
}

int SpaceShipLogic::getLives() const {
    return lives_;
}

int SpaceShipLogic::getHearts() const {
    return hearts_;
}

void SpaceShipLogic::draw(Renderer& renderer, int stage) const {
    if (stage == 0) {
        renderer.drawString(ix_, iy_, ship_row1);
        renderer.drawString(ix_, iy_ + 1, ship_row2);
        renderer.drawString(ix_, iy_ + 2, ship_row3);
    } else if (stage == 1) {
        renderer.drawString(ix_, iy_, exp1_row1);
        renderer.drawString(ix_, iy_ + 1, exp1_row2);
        renderer.drawString(ix_, iy_ + 2, exp1_row3);
    } else if (stage == 2) {
        renderer.drawString(ix_, iy_, exp2_row1);
        renderer.drawString(ix_, iy_ + 1, exp2_row2);
        renderer.drawString(ix_, iy_ + 2, exp2_row3);
    }
}
