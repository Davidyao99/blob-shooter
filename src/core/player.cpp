#include "core/player.h"

namespace shooter {

    Player::Player(glm::vec2 position, float radius,
    int hit_points, Weapon weapon) :
            Entity(position, radius, hit_points),
            weapons_(),
            curr_weapon_index_(0) {
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

    const Weapon& Player::get_curr_weapon_() const{
      return weapons_.at(curr_weapon_index_);
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

//    void Player::ChangeWeapon(bool is_next) {
//      std::cout<<curr_weapon_index_<<std::endl;
//      size_t curr_idx = curr_weapon_index_;
//      while (curr_idx < weapons_.size()) {
//        if (weapons_.at(curr_idx).get_unlocked_()) {
//          curr_weapon_index_ = curr_idx;
//          std::cout<<curr_weapon_index_<<std::endl;
//          return;
//        }
//        if (is_next) {
//          curr_idx++;
//        } else {
//          curr_idx--;
//        }
//      }
//      if (is_next) {
//        curr_idx = 0;
//      } else {
//        curr_idx = weapons_.size()-1;
//      }
//      while (curr_idx != curr_weapon_index_) {
//        if (weapons_.at(curr_idx).get_unlocked_()) {
//          curr_weapon_index_ = curr_idx;
//          std::cout<<curr_weapon_index_<<std::endl;
//          return;
//        }
//        if (is_next) {
//          curr_idx++;
//        } else {
//          curr_idx--;
//        }
//      }
//      std::cout<<curr_weapon_index_<<std::endl;
//    }

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
