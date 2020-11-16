#include "core/entity.h"

namespace shooter {

    Entity::Entity(glm::vec2 position, float radius, int hit_points, glm::vec2 velocity) :
            position_(position), radius_(radius), hit_points_(hit_points), velocity_(velocity)
    {}

    Entity::Entity(glm::vec2 position, float radius, int hit_points) :
            Entity(position, radius, hit_points, glm::vec2(0.0f, 0.0f))
    {}

    const float Entity::get_radius_() const {
        return radius_;
    }

    void Entity::Move() {
        position_ += velocity_;
    }

    const glm::vec2 Entity::get_position_() const {
        return position_;
    }

    const int Entity::get_hit_points_() const {
        return hit_points_;
    }

    bool Entity::isDead() const{
        return hit_points_ <= 0;
    }
}