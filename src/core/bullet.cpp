
#include "cinder/gl/gl.h"
#include "core/bullet.h"

namespace shooter {

    Bullet::Bullet(glm::vec2 position, ProjectileBlueprint blueprint,
               glm::vec2 cursor) :
      Entity(position, blueprint.radius_, 1, blueprint.damage_),
      is_explosive_(blueprint.isExplosive_)
      {
        velocity_ = ((cursor) / glm::length(cursor)) * blueprint.speed_;
    }

    bool Bullet::get_is_explosive_() {
      return is_explosive_;
    }

}