#include "core/weapon.h"

namespace shooter {

  Weapon::Weapon(std::string name, ProjectileType projectile_type,
               float firing_angle) :
  last_fire_(std::chrono::system_clock::now()){

  }

  float Weapon::GetReloadStatus() const {
    std::chrono::milliseconds duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - last_fire_);
    float status = duration.count() / 1000.0f;
    if (status >= 1.0f) {
      status = 1.0f;
    }
    return status;
  }

  bool Weapon::get_unlocked_() const {
    return unlocked_;
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


  }
