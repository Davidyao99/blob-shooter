
#include "cinder/gl/gl.h"
#include "core/bullet.h"

namespace shooter {

    Bullet::Bullet(glm::vec2 position, float radius, int hit_points,
               glm::vec2 cursor) : Entity(position, radius, hit_points) {
        velocity_ = ((cursor) / glm::length(cursor)) * 10.0f;
    }

}