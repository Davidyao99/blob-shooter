#pragma once

#include "core/bullet.h"
#include "core/enemy.h"
#include "core/enemy_spawner.h"
#include "core/game_config_constants.h"
#include "core/player.h"

namespace shooter {

    /**
     * Checks if two circles collide
     * @param position1 position of first circle
     * @param position2 position of second circle
     * @param radius1 radius of first circle
     * @param radius2 radius of second circle
     * @return true if collision, false otherwise
     */

    bool Collides(glm::vec2 position1, glm::vec2 position2, float radius1, float radius2);

    /**
     * Engine class that controls logic of how the game runs
     */

    class Engine {

    public:
     /**
      * Constructor, spawns player in middle of playing field. Creates
      * enemies spawns on the boundaries of the field.
      * @param length length of field, best to set in multiples of 20
      * @param height height of field, best to set in multiples of 20
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

      // getters

      const std::vector<Bullet>& get_bullets_() const;

      const std::vector<Enemy>& get_enemies_() const;

      const Player& get_player_() const;

      const glm::ivec2& get_board_dimensions_() const;

      int get_score_() const;

      const std::chrono::system_clock::time_point get_begin_time_() const;

      const std::chrono::system_clock::time_point get_last_enemy_wave_() const;

      /**
       * Gets a copy of explosions and deleted current vector of explosions
       * @return copy vector of explosions
       */

      const std::vector<std::pair<glm::vec2,float>> get_explosions_();

      /**
       * method called iteratively by cinder draw(), does moving of player,
       * moving of bullets, moving of enemies, spawning of enemies, collision
       * handling through private helper methods
       * @param moves
       */
      void update(std::set<Direction> moves);

      /**
       * Checks if bullet is long out of bound
       * @param bullet
       * @return true if bullet is out of bounds more than 500 pixels
       */

      bool IsOutOfBounds(const glm::vec2& position) const;

      /**
       * Handles collision interaction by calling HandleEnemyBulletCollision
       * and HandleEnemyPlayerCollision()
       */
      void HandleCollisions();

      /**
       * Creates an explosion at explosion_position
       * @param explosion_position
       * @param explosion_radius radius of explosion
       * @param damage
       */

      void Explode(glm::vec2 explosion_position, float explosion_radius, int damage);

      /**
       * Handle shooting
       * @param cursor takes in position of cursor so that bullet is spawn
       * with velocity heading towards cursor
       */
      ProjectileType Engine::HandleShoot(glm::vec2 cursor);

      /**
       * deals damage to all enemies in a line from player position to cursor
       * @param cursor relative position of cursor from player
       * @param projectile_radius
       * @param damage
       */

      void ShootBeam(glm::vec2 cursor, int damage);

      /**
       * Instantiates Enemy object and adds it to enemies_. Made public for
       * easier testing
       * @param enemy Enemy to be added into enemies_
       */
      void AddEnemy(Enemy enemy);

      /**
       * Instantiates Bullet object and adds it to bullets_. Made public for
       * easier testing
       * @param player_position player location, takes in argument for ease of testing
       * @param radius radius of bullet
       * @param hitpoints hitpoints of bullet
       * @param cursor cursor location for bullet to head to
       */
      void AddBullet(Bullet bullet);

      /**
       * Checks if current weapon is reloaded
       * @return true if reloaded, false otherwise
       */
      bool Reloaded() const;

      /**
       * Changes weapon for player
       * @param next true to go to next weapon, false to go to previous
       */

      void ChangeWeapon(bool next);

      /**
       * Checks if player is dead
       * @return true if player health <= 0, false otherwise
       */

      bool PlayerIsDead() const;

      /**
       * Restarts the engine by clearing all vectors, setting score to 0,
       * resetting player health and position
       */

      void Restart();

     private:

      /**
       * Create and loads in the weapons for the player
       */

      void CreateWeapons();

      /**
       * Handle Enemy and Bullet Collision. If enemy dies, bullet continues.
       * If bullet hitpoints used up, last enemy hit will get hit back.
       */
      void HandleEnemyBulletCollision();

      /**
       * Handle Enemy and Player collision. Enemy deals damage to player
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
       * Handle the deaths of entities, including enemies, bullets and players.
       * Explosive bullets that were hit will trigger explosion
       */
      void HandleDeaths();

      Player player_;
      glm::ivec2 board_dimensions_;
      EnemySpawner enemy_spawner_;
      std::vector<Bullet> bullets_;
      std::vector<Enemy> enemies_;
      std::vector<std::pair<glm::vec2,float>> explosions_;
      size_t score_;

      std::chrono::system_clock::time_point begin_time_;
      std::chrono::system_clock::time_point last_enemy_wave_;
    };
}