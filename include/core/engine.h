#pragma once

#include "core/player.h"
#include "core/bullet.h"
#include "core/enemy.h"

namespace shooter {



    class Engine {
    public:
      Engine(float length, float height);
      Engine(float length, float height, glm::vec2 player_position);
      void update(std::set<Direction> moves);
      const std::vector<Bullet>& get_bullets_() const;
      const std::vector<Enemy>& get_enemies_() const;
      const Player& get_player_() const;
      void CheckCollisions();
      void HandleShoot(glm::vec2 cursor);
      void AddEnemy(glm::vec2 position, float radius, int hit_points, float level);

    private:

      void HandleEnemyBulletCollision();
      void HandleEnemyPlayerCollision();
      void HandlePlayerAtBoundary();
      void SpawnEnemy();
      void CreateEnemySpawn();

     Player player_;
     std::vector<Bullet> bullets_;
     std::vector<Enemy> enemies_;
     std::vector<glm::vec2> enemy_spawns_;
     glm::ivec2 board_dimensions_;

     std::chrono::system_clock::time_point begin_time_;
     std::chrono::system_clock::time_point last_enemy_wave_;
    };
}