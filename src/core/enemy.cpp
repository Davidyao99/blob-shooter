#include "core/enemy.h"

namespace shooter{


    Enemy::Enemy(glm::vec2 position, float radius, int hit_points) : Entity(position, radius, hit_points) {};

    void Enemy::Accelerate(glm::vec2 player_pos) {
        glm::vec2 acc = (player_pos - position_) / glm::length(position_ - player_pos);
        velocity_ += acc * 0.2f;
        velocity_ *= 0.95f;
    }

};