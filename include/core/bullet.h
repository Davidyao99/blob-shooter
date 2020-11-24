#pragma once

#include "cinder/gl/gl.h"
#include "entity.h"

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
      * @param cursor cursor position so bullet will be spawned heading towards it
      */
     Bullet(glm::vec2 position, float radius, int hit_points,
            glm::vec2 cursor, bool isExplosive);

     bool get_is_explosive_();

    private:
     bool is_explosive_;


    };

}
