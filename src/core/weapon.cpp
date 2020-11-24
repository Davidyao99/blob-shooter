#include "core/weapon.h"

#include <iostream>

namespace shooter {

  Weapon::Weapon(std::string name, ProjectileType projectile_type,
               float firing_angle, int reload_rate) :
  last_fire_(std::chrono::system_clock::now()),
  reload_rate(reload_rate),
  name_(name),
  projectile_type_(projectile_type),
  firing_angle_(firing_angle),
  unlocked_(true){
  }

  float Weapon::GetReloadStatus() const {
    std::chrono::milliseconds duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - last_fire_);
    float status = duration.count() / static_cast<float>(reload_rate);
    if (status >= 1.0f) {
      status = 1.0f;
    }
    return status;
  }

  void Weapon::Reload() {
    last_fire_ = std::chrono::system_clock::now();
  }

  bool Weapon::get_unlocked_() const {
    return unlocked_;
  }

  bool Weapon::operator==(const Weapon rhs) const {
    return (rhs.name_ == name_ && rhs.unlocked_ == unlocked_ &&
              rhs.reload_rate == reload_rate && rhs.last_fire_ == last_fire_ &&
              rhs.firing_angle_ == firing_angle_);
  }

  std::chrono::system_clock::time_point Weapon::get_last_fire_() const {
    return last_fire_;
  }

  const std::string Weapon::get_name_() const {
    return name_;
  }

  ProjectileType Weapon::get_projectile_type_() const {
    return projectile_type_;
  }

  int Weapon::get_reload_rate() const {
    return reload_rate;
  }


  }
