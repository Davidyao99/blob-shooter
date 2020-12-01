
#include "cinder/gl/gl.h"
#include "core/bullet.h"

namespace shooter {

    Bullet::Bullet(glm::vec2 position, ProjectileBlueprint blueprint,
               glm::vec2 cursor_relative_location) :
      Entity(position, blueprint.projectile_radius_, 1, blueprint.damage_),
      is_explosive_(blueprint.isExplosive_), explosive_radius_(blueprint.explosion_radius_)
      {
        velocity_ = ((cursor_relative_location) / glm::length(cursor_relative_location))
                  * blueprint.speed_;
    }

    bool Bullet::get_is_explosive_() const {
      return is_explosive_;
    }

    float Bullet::get_explosion_radius_() const {
      return explosive_radius_;
    }

}