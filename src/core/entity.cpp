#include "core/entity.h"

namespace shooter {

    Entity::Entity(glm::vec2 position, float radius,
               int health, int damage, glm::vec2 velocity) :
            position_(position), radius_(radius), health_(health),
      damage_(damage), velocity_(velocity)
    {}

    Entity::Entity(glm::vec2 position, float radius,
                   int health, int damage) :
            Entity(position, radius, health, damage,
                 glm::vec2(0.0f, 0.0f))
    {}

    const float Entity::get_radius_() const {
        return radius_;
    }

    const int Entity::get_damage_() const {
      return damage_;
    }

    void Entity::Hit(int hit_point) {
      health_ -= hit_point;
    }

    void Entity::Move() {
        position_ += velocity_;
    }

    const glm::vec2 Entity::get_position_() const {
        return position_;
    }

    const glm::vec2 Entity::get_velocity_() const {
      return velocity_;
    }

    const int Entity::get_health_() const {
        return health_;
    }

    bool Entity::IsDead() const{
        return health_ <= 0;
    }

    void Entity::Collide(Entity& entity) {
      Hit(entity.get_damage_());
      entity.Hit(damage_);
      glm::vec2 rebound = position_ - entity.get_position_();
      velocity_ = (rebound / glm::length(rebound)) *
                  static_cast<float>(entity.get_damage_());
    }

}