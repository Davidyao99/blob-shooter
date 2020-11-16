#include "core/enemy.h"

namespace shooter{


    Enemy::Enemy(glm::vec2 position, float radius, int hit_points, float level) :
      Entity(position, radius, hit_points), level_(level) {};

    void Enemy::Accelerate(glm::vec2 player_pos) {
        glm::vec2 acc = (player_pos - position_) / glm::length(position_ - player_pos);
        velocity_ += acc * level_;
        velocity_ *= 0.95f;
    }


    void Enemy::Collide(Entity& entity) {
      int entity_hit_points = entity.get_hit_points_();
      entity.hit_points_ -= hit_points_;
      hit_points_ -= entity_hit_points;
      glm::vec2 rebound = position_ - entity.get_position_();
      velocity_ = (rebound / glm::length(rebound)) *
                  static_cast<float>(entity_hit_points);
    }
}

