#pragma once

#include "cinder/gl/gl.h"

namespace shooter{

    class Entity{

    public:
      Entity(glm::vec2 position, float radius, int hit_points, glm::vec2 velocity);
      Entity(glm::vec2 position, float radius, int hit_points);
      const float get_radius_() const;
      void Move();
      const glm::vec2 get_position_() const;
      const int get_hit_points_() const;
//        virtual void Collide(Entity& entity) = 0;
      bool IsDead() const;

      int hit_points_;


    protected:
      glm::vec2 position_;
      glm::vec2 velocity_;
      float radius_;
  };
}