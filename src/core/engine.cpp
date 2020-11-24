#include "core/engine.h"
#include <cstdlib>

namespace shooter {

    Engine::Engine(float length, float height) :
            Engine(length, height, glm::vec2(length/2, height/2)){
    }

    Engine::Engine(float length, float height, glm::vec2 player_position) :
        player_(player_position, 10.0f, 20),
        board_dimensions_(length, height), enemy_spawns_(),
        begin_time_(std::chrono::system_clock::now()),
        last_enemy_wave_(std::chrono::system_clock::now()){
      CreateEnemySpawn();

    }

    void Engine::CreateEnemySpawn() {
      int x_unit = static_cast<int>(board_dimensions_.x) / 20;
      int y_unit = static_cast<int>(board_dimensions_.y) / 20;
      for (size_t x_coord = 0; x_coord != board_dimensions_.x + x_unit;
           x_coord += x_unit) {
        enemy_spawns_.emplace_back(x_coord, 0);
        enemy_spawns_.emplace_back(x_coord, board_dimensions_.y);
      }
      for (size_t y_coord = y_unit; y_coord != board_dimensions_.y; y_coord += y_unit ) {
        enemy_spawns_.emplace_back(0, y_coord);
        enemy_spawns_.emplace_back(board_dimensions_.x, y_coord);
      }
    }

    const std::vector<glm::vec2>& Engine::get_enemy_spawns_() const {
      return enemy_spawns_;
    }

    void Engine::update(std::set<Direction> moves) {
        glm::vec2 player_pos = player_.get_position_();
        for (Direction direction: moves) {
          player_.Accelerate(direction);
        }
        HandlePlayerAtBoundary();
        player_.Move();
        for (Bullet& bullet : bullets_) {
            bullet.Move();
        }
        for (Enemy& enemy: enemies_) {
            enemy.Accelerate(player_pos);
            enemy.Move();
        }
        CheckCollisions();
        SpawnEnemy();
    }

    ProjectileType Engine::HandleShoot(glm::vec2 cursor) {
      ProjectileType type = player_.get_curr_weopon_().get_projectile_type_();
      if (type != beam) {
        AddBullet(player_.get_position_(), 10.0f,
                  15, cursor, type == explosive);
      } else {
        ShootBeam(cursor)
      }
      return type;
    }

    bool Engine::Reloaded() const {
      return player_.GetReloadStatus() == 1.0f;
    }

    void Engine::AddBullet(glm::vec2 player_position, float radius, int hitpoints,
                           glm::vec2 cursor, bool is_explosive) {
      bullets_.emplace_back(player_position,
                                radius, hitpoints, cursor, is_explosive);
    }

    void Engine::AddEnemy(glm::vec2 position, float radius,
                          int hit_points, float level) {
      enemies_.emplace_back(position,radius,hit_points,level);
    }

    void Engine::SpawnEnemy() {
      std::chrono::milliseconds duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now() - begin_time_);
      std::chrono::milliseconds time_since_last_wave =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now() - last_enemy_wave_);
      if (time_since_last_wave.count() > 100000) {
        size_t num_enemy_spawn = static_cast<size_t>(duration.count()) / 10000;
        for (num_enemy_spawn; num_enemy_spawn != 0; num_enemy_spawn--) {
          size_t index = (rand()%enemy_spawns_.size());
          AddEnemy(enemy_spawns_[index], 10.0f,
                   10, 0.3f);
        }
        last_enemy_wave_ = std::chrono::system_clock::now();
      }
    }

    const std::vector<Bullet>& Engine::get_bullets_() const {
        return bullets_;
    }

    const std::vector<Enemy>& Engine::get_enemies_() const {
        return enemies_;
    }

    const glm::ivec2& Engine::get_board_dimensions_() const {
      return board_dimensions_;
    }

    void Engine::HandlePlayerAtBoundary() {
      const glm::vec2& position = player_.get_position_();
      const glm::vec2& velocity = player_.get_velocity_();
      float radius = player_.get_radius_();
      if ((position.y - radius < 0 && velocity.y < 0) ||
          (position.y + radius > board_dimensions_.y && velocity.y > 0)) {
        player_.ZeroYVelocity();
      }

      if ((position.x - radius < 0 && velocity.x < 0) ||
          (position.x + radius > board_dimensions_.x && velocity.x > 0)) {
        player_.ZeroXVelocity();
      }
    }

    void Engine::CheckCollisions() {
      HandleEnemyBulletCollision();
      HandleEnemyPlayerCollision();
    }

  void Engine::HandleEnemyBulletCollision() {
    for (auto bullet_iter = bullets_.begin(); bullet_iter != bullets_.end();) {
      bool iterate_bullet = true;
      for (auto enemies_iter = enemies_.begin(); enemies_iter != enemies_.end();) {
        float dist = glm::length(bullet_iter->get_position_() - enemies_iter->get_position_());
        if (dist <= bullet_iter->get_radius_() + enemies_iter->get_radius_()) {
          enemies_iter->Collide(*bullet_iter);
          if (enemies_iter->IsDead()) {
            enemies_iter = enemies_.erase(enemies_iter);
          }
          if (bullet_iter->IsDead()) {
            bullet_iter = bullets_.erase(bullet_iter); // automatically iterates to next bullet
            iterate_bullet = false;
            break;
          }
        } else {
          ++enemies_iter;
        }
      }
      if (iterate_bullet) {
        ++bullet_iter;
      }
    }
  }

  const Player& Engine::get_player_() const {
    return player_;
  }

  void Engine::HandleEnemyPlayerCollision() {
    for (auto enemies_iter = enemies_.begin(); enemies_iter != enemies_.end();) {
      bool iterate_enemy = true;
      float dist = glm::length(player_.get_position_() -
                               enemies_iter->get_position_());
      if (dist <= player_.get_radius_() + enemies_iter->get_radius_()) {
        player_.Collide(*enemies_iter);
        if (enemies_iter->get_hit_points_() <= 0) {
          enemies_iter = enemies_.erase(enemies_iter);
          iterate_enemy = false;
        }
      }
      if (iterate_enemy) {
        ++enemies_iter;
      }
      if (player_.get_hit_points_() <= 0) {
        exit(1);
      }
    }
  }
}