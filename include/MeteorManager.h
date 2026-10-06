#pragma once
#include <vector>
#include <random>
#include "Renderer.h"

struct Meteor {
    int x;
    int y;
};

class MeteorManager {
public:
    MeteorManager();
    ~MeteorManager() = default;

    void reset();
    bool update();

    [[nodiscard]] int getLevel() const;
    [[nodiscard]] const std::vector<Meteor>& getMeteors() const;

    void draw(Renderer& renderer) const;

private:
    std::vector<Meteor> meteors_;
    int level_;
    int repetition_;

    std::mt19937 rng_;
    std::uniform_int_distribution<int> distX_;
    std::uniform_int_distribution<int> distY_;

    void respawnMeteor(Meteor& m);
};
