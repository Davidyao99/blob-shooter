#include "core/player.h"

namespace shooter {

    Player::Player(glm::vec2 position, float radius,
    int health, Weapon weapon) :
            Entity(position, radius, health, 0),
            weapons_(),
            curr_weapon_index_(0) { // why this doesnt work with reference variable
      weapons_.push_back(weapon);
    }

    void Player::Accelerate(Direction direction) {
      switch (direction) {
          case left:
            velocity_ += glm::vec2(-1,0);
            break;
          case up:
            velocity_ += glm::vec2(0, -1);
            break;
          case right:
            velocity_ += glm::vec2(1,0);
            break;
          case down:
            velocity_ += glm::vec2(0,1);
          break;
      }
    }

    void Player::Move() {
      position_ += velocity_;
      velocity_ *= 0.9;
      if (glm::length(velocity_) < 0.1) {
        velocity_ = glm::vec2(0,0);
      }
    }

    void Player::ReloadWeapon() {
      weapons_.at(curr_weapon_index_).Reload();
    }

    float Player::GetWeaponReloadStatus() const {
      return weapons_.at(curr_weapon_index_).GetReloadStatus();
    }

    const Weapon& Player::GetCurrentWeapon() const{
      return weapons_.at(curr_weapon_index_);
    }

    Bullet Player::FireBullet(glm::vec2 cursor) {
      Weapon weapon = GetCurrentWeapon();
      std::cout<<cursor.x<<","<<cursor.y<<std::endl;
      float r = glm::length(cursor);
      float tita = atan2(cursor.y,cursor.x); // change to polar coordinates
      float firing_angle = weapon.get_firing_angle_();
      float random_deviation = (static_cast<float>(rand())/RAND_MAX - 0.5f)*firing_angle;
      glm::vec2 new_cursor(r*cos(tita+random_deviation),
                           r*sin(tita+random_deviation));
      std::cout<<new_cursor.x<<","<<new_cursor.y<<std::endl;
      return Bullet(position_, weapon.get_projectile_blueprint_(), new_cursor);
    }

    void Player::ChangeNextWeapon() {
      size_t curr_idx = curr_weapon_index_ + 1;
      while (curr_idx < weapons_.size()) {
        if (weapons_.at(curr_idx).get_unlocked_()) {
          curr_weapon_index_ = curr_idx;
          std::cout << curr_weapon_index_ << std::endl;
          return;
        }
        curr_idx++;
      }
      curr_idx = 0;
      while (curr_idx != curr_weapon_index_) {
        if (weapons_.at(curr_idx).get_unlocked_()) {
          curr_weapon_index_ = curr_idx;
          std::cout<<curr_weapon_index_<<std::endl;
          return;
        }
        curr_idx++;
      }
    }

    void Player::ChangePrevWeapon() {
      size_t curr_idx = curr_weapon_index_ - 1;
      while (curr_idx < weapons_.size()) {
        if (weapons_.at(curr_idx).get_unlocked_()) {
          curr_weapon_index_ = curr_idx;
          std::cout << curr_weapon_index_ << std::endl;
          return;
        }
        curr_idx--;
      }
      curr_idx = weapons_.size() - 1;
      while (curr_idx != curr_weapon_index_) {
        if (weapons_.at(curr_idx).get_unlocked_()) {
          curr_weapon_index_ = curr_idx;
          std::cout<<curr_weapon_index_<<std::endl;
          return;
        }
        curr_idx--;
      }
    }

    void Player::AddWeapon(Weapon weapon) {
      weapons_.push_back(weapon);
    }

    void Player::ZeroXVelocity() {
      velocity_.x = 0;
    }

    void Player::ZeroYVelocity() {
      velocity_.y = 0;
    }

}
