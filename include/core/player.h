#pragma once

#include "cinder/gl/gl.h"
#include "entity.h"

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
      Player(glm::vec2 position, float radius, int hit_points);

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
       * Checks if last_fire_ is > 1 sec ago
       * @return true if last_fire_ > 1 sec ago, false otherwise
       */
      bool Shoot();

      /**
       * Zero X component of Velocity, to be called when player at right
       * or left boundary
       */
      void ZeroXVelocity();

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
      float GetReloadStatus() const;

    private:
        std::chrono::system_clock::time_point last_fire_;
    };
}