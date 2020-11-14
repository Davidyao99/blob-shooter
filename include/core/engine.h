#pragma once

#include "core/player.h"
#include "core/bullet.h"
#include "core/enemy.h"

namespace shooter {



    class Engine {
    public:
        Engine();
        void update(std::set<Direction> moves);
        const glm::vec2 GetPlayerPosition() const;
        void spawnBullet(glm::vec2 cursor);
        const std::vector<Bullet> get_bullets_() const;
        const std::vector<Enemy> get_enemies_() const;
        void CheckCollisions();
        void HandleShoot(glm::vec2 cursor);

    private:
        Player player_;
        std::vector<Bullet> bullets_;
        std::vector<Enemy> enemies_;
    };
}