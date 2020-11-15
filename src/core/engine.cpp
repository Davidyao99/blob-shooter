#include "core/engine.h"

namespace shooter {

    Engine::Engine(float length, float height) :
            player_(glm::vec2 (0,0), 10.0f, 10),
          board_dimensions_(length, height){
        Enemy enemy1(glm::vec2 (50,50),10.0f, 10, 0.2f);
        Enemy enemy2(glm::vec2 (200,200), 10.0f, 10, 0.3f);
        Enemy enemy3(glm::vec2 (300,300), 10.0f, 10, 1.0f);
        enemies_.push_back(enemy1);
        enemies_.push_back(enemy2);
        enemies_.push_back(enemy3);
    }

    const glm::vec2 Engine::GetPlayerPosition() const {
        return player_.get_position_();
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
    }

    void Engine::HandleShoot(glm::vec2 cursor) {
        if (player_.Shoot()) {
            shooter::Bullet bullet(player_.get_position_(), 10.0f, 1, cursor);
            bullets_.push_back(bullet);
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