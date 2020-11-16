#pragma once

#include "core/player.h"
#include "core/bullet.h"
#include "core/enemy.h"

namespace shooter {



    class Engine {
    public:
        Engine(float length, float height);
        void update(std::set<Direction> moves);
        const glm::vec2 GetPlayerPosition() const;
        void spawnBullet(glm::vec2 cursor);
        const std::vector<Bullet> get_bullets_() const;
        const std::vector<Enemy> get_enemies_() const;
        void CheckCollisions();
        void HandleShoot(glm::vec2 cursor);
        void HandlePlayerAtBoundary();
        void SpawnEnemy();
        void CreateEnemySpawn();

    private:

     void HandleEnemyBulletCollision();

     void HandleEnemyPlayerCollision();

     Player player_;
     std::vector<Bullet> bullets_;
     std::vector<Enemy> enemies_;
     std::vector<glm::vec2> enemy_spawns_;
     glm::ivec2 board_dimensions_;

     std::chrono::system_clock::time_point begin_time_;
     std::chrono::system_clock::time_point last_enemy_wave_;
    };
}