#include "core/engine.h"
#include <cstdlib>

namespace shooter {

  bool Collides(glm::vec2 position1, glm::vec2 position2,
              float radius1, float radius2) {
    float dist = glm::length(position1 - position2);
    return dist <= radius1 + radius2;
  }


  Engine::Engine(float length, float height)
      : Engine(length, height, glm::vec2(length / 2, height / 2)) {
  }

  Engine::Engine(float length, float height, glm::vec2 player_position)
      : player_(player_position, 10.0f, 50),
        board_dimensions_(length, height),
        begin_time_(std::chrono::system_clock::now()),
        last_enemy_wave_(std::chrono::system_clock::now()),
        explosions_(),
        score_(0),
        enemy_spawner_(board_dimensions_){
    CreateWeapons();
  }

  void Engine::CreateWeapons() {
    player_.AddWeapon(kSniper);
    player_.AddWeapon(kMachineGun);
    player_.AddWeapon(kRifle);
    player_.AddWeapon(kLaser);
    player_.AddWeapon(kRocket);
  }

  void Engine::update(std::set<Direction> moves) {
    MoveAllEntities(moves);
    HandlePlayerAtBoundary();
    HandleCollisions();
    SpawnEnemy();
    HandleDeaths();
  }

  void Engine::MoveAllEntities(std::set<Direction> moves) {
    glm::vec2 player_pos = player_.get_position_();
    for (Direction direction : moves) {
      player_.Accelerate(direction);
    }
    player_.Move();
    for (Bullet& bullet : bullets_) {
      bullet.Move();
    }
    for (Enemy& enemy : enemies_) {
      enemy.Accelerate(player_pos);
      enemy.Move();
    }
  }

  bool Engine::PlayerIsDead() const {
    return player_.get_health_() <= 0;
  }

  void Engine::Restart() {
    enemies_.clear();
    bullets_.clear();
    score_ = 0;
    explosions_.clear();
    player_.Reset(glm::vec2(board_dimensions_.x / 2, board_dimensions_.y / 2),50);
    begin_time_ = std::chrono::system_clock::now();
    last_enemy_wave_ = std::chrono::system_clock::now();
  }

  const std::chrono::system_clock::time_point Engine::get_begin_time_() const {
    return begin_time_;
  }
  const std::chrono::system_clock::time_point Engine::get_last_enemy_wave_() const {
    return last_enemy_wave_;
  }

  void Engine::HandleDeaths() {
    for (size_t bullet_idx = bullets_.size() - 1; bullet_idx < bullets_.size();
         bullet_idx--) {

      glm::ivec2 explosion_position;
      float explosion_radius;

      if (bullets_.at(bullet_idx).IsDead()) {
        if (bullets_.at(bullet_idx).get_is_explosive_()) { // explode upon death
          explosion_position = bullets_.at(bullet_idx).get_position_();
          explosion_radius = bullets_.at(bullet_idx).get_explosion_radius_();
          Explode(explosion_position,
                  explosion_radius,
                  bullets_.at(bullet_idx).get_damage_());
          explosions_.push_back(std::pair<glm::ivec2, float>(explosion_position,
                                                            explosion_radius));
        }
        bullets_.erase(bullets_.begin() + bullet_idx);
      } else if (IsOutOfBounds(bullets_.at(bullet_idx).get_position_())) {
        bullets_.erase(bullets_.begin() + bullet_idx);
      }
    }
    for (size_t enemy_idx = enemies_.size() - 1; enemy_idx < enemies_.size();
         enemy_idx--) {
      if (enemies_.at(enemy_idx).IsDead()) {
        enemies_.erase(enemies_.begin() + enemy_idx);
        score_ += 10;
      }
    }
  }

  bool Engine::IsOutOfBounds(const glm::vec2& position) const {
    return position.x > board_dimensions_.x + 500 ||
           position.x < - 500 ||
           position.y > board_dimensions_.y +500 ||
           position.y < -500;
  }

  ProjectileType Engine::HandleShoot(glm::vec2 cursor_relative_to_player_pos) {
    if (!Reloaded()) {
      return bullet; // nothing happens
    }
    const Weapon& weapon = player_.GetCurrentWeapon();
    ProjectileType type = weapon.get_projectile_type_();
    if (type != beam) {
      Bullet bullet = player_.FireBullet(cursor_relative_to_player_pos);
      AddBullet(bullet);
    } else {
      ShootBeam(cursor_relative_to_player_pos,
                weapon.get_projectile_blueprint_().damage_);
    }
    player_.ReloadWeapon();  // reset reload timing
    return type;
  }

  void Engine::ShootBeam(glm::vec2 cursor_relative_to_player_pos,
                         int damage) {
    glm::vec2 laser_unit_vector = cursor_relative_to_player_pos /
                                  glm::length(cursor_relative_to_player_pos);
    for (auto& enemy : enemies_) {
      glm::vec2 player_to_enemy = enemy.get_position_() - player_.get_position_();
      float player_to_enemy_dist = glm::length(player_to_enemy);
      float dot_product = glm::dot(player_to_enemy, laser_unit_vector);
      if (dot_product <= 0) {  // ensure beam does not shoot backwards
        continue;
      }
      float perp_dist = sqrt(pow(player_to_enemy_dist, 2) - pow(dot_product, 2));
      if (perp_dist <= enemy.get_radius_()) {
        // push enemy away from source of laser
        enemy.Hit(damage,
                  enemy.get_position_() - laser_unit_vector);
      }
    }
  };

  bool Engine::Reloaded() const {
    return player_.GetWeaponReloadStatus() == 1.0f;
  }

  void Engine::AddBullet(Bullet bullet) {
    bullets_.emplace_back(bullet);
  }

  void Engine::AddEnemy(Enemy enemy) {
    enemies_.push_back(enemy);
  }

  void Engine::ChangeWeapon(bool next) {
    if (next) {
      player_.ChangeNextWeapon();
    } else {
      player_.ChangePrevWeapon();
    }
  }

  void Engine::SpawnEnemy() {
    std::chrono::milliseconds duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - begin_time_);
    std::chrono::milliseconds time_since_last_wave =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - last_enemy_wave_);
    std::vector<Enemy> enemies = enemy_spawner_.SpawnEnemies(static_cast<long>(duration.count()),
                                                           static_cast<long>(time_since_last_wave.count()));
    if (enemies.size() == 0) {
      return;
    }
    last_enemy_wave_ = std::chrono::system_clock::now();
    for (Enemy enemy: enemies) {
      AddEnemy(enemy);
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

  int Engine::get_score_() const {
    return score_;
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

  const std::vector<std::pair<glm::vec2,float>> Engine::get_explosions_() {
    std::vector<std::pair<glm::vec2,float>> explosions = explosions_;
    explosions_.clear();
    return explosions;
  }

//  void Engine::ClearExplosions() {
//    explosions_.clear();
//  }

  void Engine::HandleCollisions() {
    HandleEnemyBulletCollision();
    HandleEnemyPlayerCollision();
  }

  void Engine::Explode(glm::vec2 explosion_position, float explosion_radius,
                       int damage) {
    for (auto& enemy : enemies_) {
      if(Collides(enemy.get_position_(),explosion_position,
                   enemy.get_radius_(), explosion_radius)) {
        enemy.Hit(damage, explosion_position);
      }
    }
    if (Collides(player_.get_position_(), explosion_position,
                 player_.get_radius_(), explosion_radius)) {
      player_.Hit(damage / 5, explosion_position);
    }
  }

  void Engine::HandleEnemyBulletCollision() {
    for (auto& bullet : bullets_) {
      for (auto& enemy : enemies_) {
        if (Collides(bullet.get_position_(), enemy.get_position_(),
                     bullet.get_radius_(), enemy.get_radius_())) {
          if (bullet.get_is_explosive_()) {
            bullet.Hit(bullet.get_health_(),
                       enemy.get_position_()); // kills bullet so it can explode
          } else {
            enemy.Collide(bullet);
          }
          break;
        }
      }
    }
  }

  const Player& Engine::get_player_() const {
    return player_;
  }

  void Engine::HandleEnemyPlayerCollision() {
    for (auto& enemy : enemies_) {
      if (Collides(enemy.get_position_(), player_.get_position_(),
                   enemy.get_radius_(), player_.get_radius_())) {
        player_.Collide(enemy);
      }
    }
  }
} // namespace shooter