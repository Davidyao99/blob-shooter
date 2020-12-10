#pragma once

#include <chrono>
#include <string>

#include "projectile_blueprint.h"

namespace shooter {

/**
 * To differentiate the 2 types of firables. Firing a beam will not actually
 * create any object whereas firing a bullet will create a Bullet.
 */

enum ProjectileType {
  kBeam, // for beam projectile_blueprint, firing angle, radius and
  // speed is irrelevant
  kBullet
};

/**
 * Weapon class to represent the different types of weapons and their
 * attributes
 */

class Weapon {

 public:

  /**
   * Constructor for weapon
   * @param name
   * @param projectile_type
   * @param firing_angle angle in radians of the spread of bullets
   * @param reload_rate
   * @param projectile_blueprint
   */

  Weapon(std::string name, ProjectileType projectile_type,
         float firing_angle, int reload_rate,
         ProjectileBlueprint projectile_blueprint);

  // getters

  std::chrono::system_clock::time_point get_last_fire_() const;

  const std::string get_name_() const;

  ProjectileType get_projectile_type_() const;

  int get_reload_rate_() const;

  ProjectileBlueprint get_projectile_blueprint_() const;

  float get_firing_angle_() const;

  /**
   * equality operator to be used for testing
   * @param rhs
   * @return true if rhs weapon is same as this weapon object
   */

  bool operator==(const Weapon rhs) const;

  /**
   * Retrieves the reload status of current weapon
   * @return float from 0 - 1.0f with 1.0f meaning reloaded
   */

  float GetReloadStatus() const;

  /**
   * Reload current weapon by resetting last_fire_
   */

  void Reload();

 private:

  std::chrono::system_clock::time_point last_fire_;

  std::string name_;

  ProjectileType projectile_type_;

  int reload_rate;

  float firing_angle_; // spread of bullet

  ProjectileBlueprint projectile_blueprint_;
};

} // namespace shooter
