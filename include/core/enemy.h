#pragma once

#include "cinder/gl/gl.h"
#include "entity.h"

namespace shooter{

    class Enemy : public Entity {
    public:

        Enemy(glm::vec2 position, float radius, int hit_points, float level);
        void Enemy::Accelerate(glm::vec2 player_pos);

    private:

     float level_;

    };
}