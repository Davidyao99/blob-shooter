#pragma once

#include "cinder/gl/gl.h"
#include "entity.h"

namespace shooter {

    class Bullet : public Entity{
    public:
        Bullet(glm::vec2 position, float radius, int hit_points, glm::vec2 cursor);
        void Collide(Entity& entity);
    private:
    };
}
