#include "core/engine.h"
#include <cstdlib>

namespace shooter {

    Engine::Engine(float length, float height) :
            player_(glm::vec2 (0,0), 10.0f, 10),
          board_dimensions_(length, height), enemy_spawns_(),
          begin_time_(std::chrono::system_clock::now()),
          last_enemy_wave_(std::chrono::system_clock::now()){
        CreateEnemySpawn();
    }

    const glm::vec2 Engine::GetPlayerPosition() const {
        return player_.get_position_();
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

    void Engine::HandleShoot(glm::vec2 cursor) {
        if (player_.Shoot()) {
            shooter::Bullet bullet(player_.get_position_(), 10.0f,
                                 1, cursor);
            bullets_.push_back(bullet);
        }
    }

    void Engine::SpawnEnemy() {
      std::chrono::milliseconds duration =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now() - begin_time_);
      std::chrono::milliseconds time_since_last_wave =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now() - last_enemy_wave_);
      if (time_since_last_wave.count() > 10000) {
        size_t num_enemy_spawn = static_cast<size_t>(duration.count()) / 10000;
        std::cout<<num_enemy_spawn<<std::endl;
        for (num_enemy_spawn; num_enemy_spawn != 0; num_enemy_spawn--) {
          size_t index = (rand()%enemy_spawns_.size());
          enemies_.push_back(Enemy(enemy_spawns_[index], 10.0f,
                                   10, 0.3f));
        }
        last_enemy_wave_ = std::chrono::system_clock::now();
      }

    }

    const std::vector<Bullet> Engine::get_bullets_() const {
        return bullets_;
    }

    const std::vector<Enemy> Engine::get_enemies_() const {
        return enemies_;
    }

    void Engine::HandlePlayerAtBoundary() {
      const glm::vec2& position = player_.get_position_();
      const glm::vec2& velocity = player_.get_velocity();
      float radius = player_.get_radius_();
      if ((position.y - radius <= 0 && velocity.y < 0) ||
          (position.y + radius >= board_dimensions_.y && velocity.y > 0)) {
        player_.ZeroY();
      }

      if ((position.x - radius <= 0 && velocity.x < 0) ||
          (position.x + radius >= board_dimensions_.x && velocity.x > 0)) {
        player_.ZeroX();
      }
    }

    void Engine::CheckCollisions() {

        for (auto bullet_iter = bullets_.begin(); bullet_iter != bullets_.end();) {
            bool b = true;
            for (auto enemies_iter = enemies_.begin(); enemies_iter != enemies_.end();) {
                float dist = glm::length(bullet_iter->get_position_() - enemies_iter->get_position_());
                if (dist <= bullet_iter->get_radius_() + enemies_iter->get_radius_()) {
                    std::cout<<bullet_iter->get_hit_points_()<<std::endl;
                    enemies_iter->Collide(*bullet_iter);
                    if (bullet_iter->isDead()) {
                        bullet_iter = bullets_.erase(bullet_iter);
                        b = false;
                        break;
                    }
                    if (enemies_iter->isDead()) {
                        enemies_iter = enemies_.erase(enemies_iter);
                    }
                } else {
                    ++enemies_iter;
                }
            }
            if (b) {
                ++bullet_iter;
            }

        }
    }
}