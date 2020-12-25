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

    void Entity::Hit(int hit_point, glm::vec2 impact_position) {
      health_ -= hit_point;
      glm::vec2 rebound = position_ - impact_position;
      velocity_ += (rebound / glm::length(rebound)) *
                  static_cast<float>(hit_point);
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
      Hit(entity.get_damage_(), entity.get_position_());
      entity.Hit(damage_, position_);
    }

    bool Entity::operator==(const Entity& other) const {
      if (&other == this) {
        return true;
      }
      bool same_position = (glm::length(this->position_ - other.position_) <
                            0.001f);
      bool same_velocity = (glm::length(this->velocity_ - other.velocity_) <
                            0.001f);
      return (same_position &&
              same_velocity &&
              this->radius_ == other.radius_ &&
              this->health_ == other.health_ &&
              this->damage_ == other.damage_);

    }

}