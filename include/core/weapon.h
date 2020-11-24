#pragma once

#include <chrono>
#include <string>

namespace shooter {

  enum ProjectileType {
    explosive,
    beam,
    bullet
  };

  class Weapon {

   public:
    Weapon(std::string name, ProjectileType projectile_type,
           float firing_angle, int reload_rate);

    std::chrono::system_clock::time_point get_last_fire_() const;
    const std::string get_name_() const;
    ProjectileType get_projectile_type_() const;
    bool get_unlocked_() const;
    int get_reload_rate() const;

    bool operator==(const Weapon rhs) const;

    float GetReloadStatus() const;

    void Reload();



   private:
    std::chrono::system_clock::time_point last_fire_;
    std::string name_;
    ProjectileType projectile_type_;
    bool unlocked_;
    int reload_rate;
    float firing_angle_;
  };
}
