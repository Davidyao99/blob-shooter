#pragma once

#include "bullet.h"
#include "cinder/gl/gl.h"
#include "entity.h"
#include "weapon.h"

namespace shooter {

    enum Direction {
        left,
        up,
        right,
        down
    };

    /**
     * Player class
     */

    class Player : public Entity {

    public:
      Player(glm::vec2 position, float radius,
             int health);

      /**
       * Override Move method from Entity to simulate friction by
       * multiplying 0.9 to velocity every call
       */
      void Move();

      /**
       * Accelerates in direction
       * @param direction direction to accelerate towards
       */
      void Accelerate(Direction direction);

      /**
       * Returns a const reference to current weapon
       * @return const reference of current weapon
       */

      const Weapon& GetCurrentWeapon() const;

      /**
       * Zero X component of Velocity, to be called when player at right
       * or left boundary
       */
      void ZeroXVelocity();

      /**
       * Reloads the weapon by resetting the last_fire variable
       */

      void ReloadWeapon();

      /**
       * Zero Y component of velocity, to be called when player at top or
       * bottom boundary
       */
      void ZeroYVelocity();

      /**
       * Get duration in percentage out of 1 sec from last_fire. If more than
       * 1 sec ago, return 1
       * @return
       */
      float GetWeaponReloadStatus() const;

      /**
       * Creates a bullet using current weapon towards cursor direction
       * @param cursor relative position of cursor to player
       * @return bullet
       */

      Bullet FireBullet(glm::vec2 cursor);

      /**
       * Adds weapon to weapons
       */
      void AddWeapon(Weapon weapon);

      /**
       * Switches to next weapon
       */

      void ChangeNextWeapon();

      /**
       * Switches to prev weapon
       */

      void ChangePrevWeapon();

    private:
        std::vector<Weapon> weapons_;
        int curr_weapon_index_;

    };
}