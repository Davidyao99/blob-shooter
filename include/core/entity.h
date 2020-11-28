#pragma once

#include "cinder/gl/gl.h"

namespace shooter{

    /**
     * Parent class for enemy, player and bullet. Basic object with
     * common methods such as moving, collision, isDead and common
     * variables such as position, velocity and radius
     */

    class Entity{

    public:

      Entity(glm::vec2 position, float radius, int health, int damage, glm::vec2 velocity);
      Entity(glm::vec2 position, float radius, int health, int damage);

      // getters
      const float get_radius_() const;
      const glm::vec2 get_position_() const;
      const glm::vec2 get_velocity_() const;
      const int get_health_() const;
      const int get_damage_() const;

      /**
       * Move method that adds velocity to position to move entity
       */
      void Move();

      /**
       * Collides entity with other entity. Minus hitpoints from both entity
       * and adds rebound to this entity
       * @param entity other entity that is colliding into this entity
       */
      void Collide(Entity& entity);

      /**
       * boolean to check if hitpoints <= 0
       * @return true if hitpoints <= 0, false otherwise
       */
      bool IsDead() const;

      void Hit(int hit_point, glm::vec2 impact_position);

      int health_;

      int damage_;

    protected:
      glm::vec2 position_;
      glm::vec2 velocity_;
      float radius_;
  };
}