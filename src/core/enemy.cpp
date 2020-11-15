#include "core/enemy.h"

namespace shooter{


    Enemy::Enemy(glm::vec2 position, float radius, int hit_points, float level) :
      Entity(position, radius, hit_points), level_(level) {};

    void Enemy::Accelerate(glm::vec2 player_pos) {
        glm::vec2 acc = (player_pos - position_) / glm::length(position_ - player_pos);
        velocity_ += acc * level_;
        velocity_ *= 0.95f;
    }

};