#include "core/player.h"

namespace shooter {

    Player::Player(glm::vec2 position, float radius,
    int hit_points) :
            Entity(position, radius, hit_points),
            curr_weapon_(weapons_.at(0)) {
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

    const Weapon& Player::get_curr_weopon_() const{
      return curr_weapon_;
    }

    void Player::ChangeWeapon(bool is_next) {
      auto curr_iter = std::find(weapons_.begin(),
                                 weapons_.end(), curr_weapon_);
      while (curr_iter != weapons_.end()) {
        if (curr_iter->get_unlocked_()) {
          curr_weapon_ = *curr_iter;
          return;
        }
        if (is_next) {
          curr_iter++;
        } else {
          curr_iter--;
        }
      }
      auto curr_iter_2 = curr_iter;
      if (is_next) {
        curr_iter_2 = weapons_.begin();
      } else {
        curr_iter_2 = weapons_.end();
      }
      while (curr_iter_2 != curr_iter) {
        if (curr_iter->get_unlocked_()) {
          curr_weapon_ = *curr_iter;
          return;
        }
        if (is_next) {
          curr_iter++;
        } else {
          curr_iter--;
        }
      }
    }

    void Player::AddWeapon(Weapon weapon) {
      weapons_.push_back(weapon)
    }

    void Player::ZeroXVelocity() {
      velocity_.x = 0;
    }

    void Player::ZeroYVelocity() {
      velocity_.y = 0;
    }

}
