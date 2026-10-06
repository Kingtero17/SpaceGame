#include "MeteorManager.h"

MeteorManager::MeteorManager() 
    : level_(1), 
      repetition_(0), 
      rng_(std::random_device{}()), 
      distX_(6, 115),
      distY_(0, 4)
{
    reset();
}

void MeteorManager::reset() {
    level_ = 1;
    repetition_ = 0;
    
    meteors_ = {
        {36, 10}, {82, 9}, {90, 8}, {74, 4}, {58, 7},
        {42, 3},  {66, 2}, {50, 5}, {28, 6}, {98, 1}
    };
}

bool MeteorManager::update() {
    bool levelUp = false;
    
    for (auto& m : meteors_) {
        m.y++;
    }

    for (size_t i = 0; i < meteors_.size(); ++i) {
        if (meteors_[i].y > 20) {
            respawnMeteor(meteors_[i]);
            
            if (i == 0) {
                repetition_++;
                if (repetition_ >= 20) {
                    level_++;
                    repetition_ = 0;
                    levelUp = true;
                }
            }
        }
    }
    
    return levelUp;
}

void MeteorManager::respawnMeteor(Meteor& m) {
    m.y = distY_(rng_);
    m.x = distX_(rng_);
}

int MeteorManager::getLevel() const {
    return level_;
}

const std::vector<Meteor>& MeteorManager::getMeteors() const {
    return meteors_;
}

void MeteorManager::draw(Renderer& renderer) const {
    for (const auto& m : meteors_) {
    
#ifdef _WIN32
        renderer.drawChar(m.x, m.y, static_cast<char>(1)); 
#else
        renderer.drawChar(m.x, m.y, 'O');
#endif
    }
}
