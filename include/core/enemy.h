#pragma once

#include "cinder/gl/gl.h"
#include "entity.h"
#include "bullet.h"

namespace shooter{

    /**
    * Class to represent Enemy, extends from entity
    */

    class Enemy : public Entity {

    public:

     /**
      * Constructor for Enemy
      * @param position position of enemy
      * @param radius radius of enemy
      * @param hit_points life for enemy
      * @param damage damage that enemy deals
      * @param level how fast the enemy follows the player, 0.0f to 1.0f with
      * 1.0f being fastest
      */

     Enemy(glm::vec2 position, float radius, int health, int damage, float level);

     /**
      * Changes the velocity of enemy so that it follows player. Speed of
      * acceleration depends on level_
      * @param player_pos position of player
      */
     void Accelerate(glm::vec2 player_pos);

    private:

     float level_;

    };
}