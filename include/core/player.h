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

    class Player : public Entity {

    public:
      Player(glm::vec2 position, float radius, int hit_points);
      const glm::vec2 get_velocity() const;
      void Move();
      void Accelerate(Direction direction);
      bool Shoot();
      void ZeroX();
      void ZeroY();
      void Collide(Entity& entity);
      float GetReloadStatus() const;

    private:
        std::chrono::system_clock::time_point last_fire_;
    };
}