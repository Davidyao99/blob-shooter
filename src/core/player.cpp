#include "core/player.h"

namespace shooter {

Player::Player(glm::vec2 position, float radius,
               int health) :
    Entity(position, radius, health, 10),
    weapons_(),
    curr_weapon_index_(0) {
}

void Player::Accelerate(Direction direction) {
  switch (direction) {
    case kLeft:
      velocity_ += glm::vec2(-1,0);
      break;
    case kUp:
      velocity_ += glm::vec2(0, -1);
      break;
    case kRight:
      velocity_ += glm::vec2(1,0);
      break;
    case kDown:
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

void Player::Reset(const glm::vec2 &position, int health) {
  position_ = position;
  health_ = health;
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

Bullet Player::FireBullet(const glm::vec2 &cursor) {
  Weapon weapon = GetCurrentWeapon();
  float r = glm::length(cursor);
  float tita = atan2(cursor.y,cursor.x); // change to polar coordinates
  float firing_angle = weapon.get_firing_angle_();
  float random_deviation = (static_cast<float>(rand())/RAND_MAX - 0.5f)*firing_angle;
  glm::vec2 new_cursor(r*cos(tita+random_deviation),
                       r*sin(tita+random_deviation));
  return Bullet(position_, weapon.get_projectile_blueprint_(), new_cursor);
}

void Player::ChangeNextWeapon() {
  if (curr_weapon_index_ == weapons_.size() - 1) {
    curr_weapon_index_ = 0;
    return;
  }
  curr_weapon_index_++;
}

void Player::ChangePrevWeapon() {
  if (curr_weapon_index_ == 0) {
    curr_weapon_index_ = weapons_.size() - 1;
    return;
  }
  curr_weapon_index_--;
}

void Player::AddWeapon(const Weapon &weapon) {
  weapons_.push_back(weapon);
}

void Player::ZeroXVelocity() {
  velocity_.x = 0;
}

void Player::ZeroYVelocity() {
  velocity_.y = 0;
}

} // namespace shooter
