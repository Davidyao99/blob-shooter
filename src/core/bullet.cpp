
#include "cinder/gl/gl.h"
#include "core/bullet.h"

namespace shooter {

    Bullet::Bullet(glm::vec2 position, float radius, int hit_points,
               glm::vec2 cursor, bool is_explosive_) :
      Entity(position, radius, hit_points),
      is_explosive_(is_explosive_),
      {
        velocity_ = ((cursor) / glm::length(cursor)) * 10.0f;
    }

    bool Bullet::get_is_explosive_() {
      return is_explosive_;
    }

}