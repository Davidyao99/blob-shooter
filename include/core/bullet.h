#pragma once

#include "cinder/gl/gl.h"
#include "entity.h"
#include "projectile_blueprint.h"

namespace shooter {

/**
     * Bullet class to represent bullets fired by player, extends from Entity
     */

class Bullet : public Entity {

 public:

  /**
   * Bullet constructor
   * @param position
   * @param blueprint
   * @param cursor_relative_location relative location of cursor from parameter position
   */

  Bullet(glm::vec2 position, ProjectileBlueprint blueprint,
         glm::vec2 cursor_relative_location);

  // getters
  bool get_is_explosive_() const;

  float get_explosion_radius_() const;

 private:

  bool is_explosive_;
  float explosive_radius_;

};

} // namespace shooter
