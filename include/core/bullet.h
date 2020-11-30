#pragma once

#include "cinder/gl/gl.h"
#include "entity.h"
#include "projectile_blueprint.h"

namespace shooter {

    /**
     * Bullet class to represent bullets fired by player, extends from Entity
     */

    class Bullet : public Entity{

    public:

     /**
      * Constructor for Bullet class
      * @param position position of bullet to be spawned at
      * @param radius radius of bullet
      * @param hit_points hit points of bullet
      * @param cursor relative position of cursor to player
      */
     Bullet(glm::vec2 position, ProjectileBlueprint blueprint,
            glm::vec2 cursor);

     bool get_is_explosive_() const;

     float get_explosion_radius_() const;

     private:
      bool is_explosive_;
      float explosive_radius_;

    };

}
