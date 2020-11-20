#pragma once

#include "core/player.h"
#include "core/bullet.h"
#include "core/enemy.h"

namespace shooter {

    /**
     * Engine class that controls logic of how the game runs
     */

    class Engine {
    public:
     /**
      * Constructor, spawns player in middle of playing field. Creates
      * enemies spawns on the boundaries of the field.
      * @param length length of field
      * @param height height of field
      */
     Engine(float length, float height);

     /**
      * Constructor used for testing. Creates enemies spawns on the boundaries
      * of the field.
      * @param length length of field
      * @param height height of field
      * @param player_position position to place player
      */

      Engine(float length, float height, glm::vec2 player_position);

      /**
       * method called iteratively by cinder draw(), does moving of player,
       * moving of bullets, moving of enemies, spawning of enemies, collision
       * handling through private helper methods
       * @param moves
       */
      void update(std::set<Direction> moves);

      // getters
      const std::vector<Bullet>& get_bullets_() const;
      const std::vector<Enemy>& get_enemies_() const;
      const Player& get_player_() const;
      const std::vector<glm::vec2>& get_enemy_spawns_() const;
      const glm::ivec2& get_board_dimensions_() const;

      /**
       * Handles collision interaction by calling HandleEnemyBulletCollision
       * and HandleEnemyPlayerCollision()
       */
      void CheckCollisions();

      /**
       * Handle shooting
       * @param cursor takes in position of cursor so that bullet is spawn
       * with velocity heading towards cursor
       */
      void HandleShoot(glm::vec2 cursor);

      /**
       * Instantiates Enemy object and adds it to enemies_. Made public for
       * easier testing
       * @param position position of enemy spawned
       * @param radius radius of enemy
       * @param hit_points hit_points of enemy
       * @param level how fast enemy follows player
       */
      void AddEnemy(glm::vec2 position, float radius, int hit_points, float level);

      /**
       * Instantiates Bullet object and adds it to bullets_. Made public for
       * easier testing
       * @param player_position player location, takes in argument for ease of testing
       * @param radius radius of bullet
       * @param hitpoints hitpoints of bullet
       * @param cursor cursor location for bullet to head to
       */
      void AddBullet(glm::vec2 player_position, float radius, int hitpoints,
                             glm::vec2 cursor);

    private:

      /**
       * Handle Enemy and Bullet Collision. If enemy dies, bullet continues.
       * If bullet hitpoints used up, last enemy hit will get hit back.
       */
      void HandleEnemyBulletCollision();

      /**
       * Handle Enemy and Player collision. Enemy deals damager to player
       * that is equivalent to health of enemy. Player gets knocked back on
       * impact.
       */
      void HandleEnemyPlayerCollision();

      /**
       * Prevents player from moving out of boundary. Changes velocity in that
       * axis to 0.
       */
      void HandlePlayerAtBoundary();

      /**
       * Spawns enemy randomly at one of the locations in enemy_spawns
       */
      void SpawnEnemy();

      /**
       * Creates enemy spawns on the boundaries of the map. Called during
       * construction of engine. 21 spawns on each side.
       */
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