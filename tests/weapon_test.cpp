#include <catch2/catch.hpp>
#include "core/weapon.h"
#include <chrono>
#include <thread>

using shooter::Weapon;
using shooter::ProjectileBlueprint;
using shooter::kBullet;
using shooter::kBeam;

TEST_CASE("weapon == operator works accordingly") {

  SECTION("== returns true when all variables are the same") {
    ProjectileBlueprint blueprint1(1.0f, 10, 1.0f,
                                  false, 0.0f);
    Weapon weapon1("test", kBullet, 1.0f,
                   20, blueprint1);

    ProjectileBlueprint blueprint2(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon2("test", kBullet, 1.0f,
                   20, blueprint2);

    REQUIRE((weapon1 == weapon2));
  }

  SECTION("== returns false when name is different") {
    ProjectileBlueprint blueprint1(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon1("test1", kBullet, 1.0f,
                   20, blueprint1);

    ProjectileBlueprint blueprint2(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon2("test2", kBullet, 1.0f,
                   20, blueprint2);
    REQUIRE_FALSE((weapon1 == weapon2));
  }

  SECTION("== returns false when projectile_type is different") {
    ProjectileBlueprint blueprint1(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon1("test", kBullet, 1.0f,
                   20, blueprint1);

    ProjectileBlueprint blueprint2(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon2("test", kBeam, 1.0f,
                   20, blueprint2);
    REQUIRE_FALSE((weapon1 == weapon2));
  }

  SECTION("== returns false when firing_angle is different") {
    ProjectileBlueprint blueprint1(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon1("test", kBullet, 1.0f,
                   20, blueprint1);

    ProjectileBlueprint blueprint2(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon2("test", kBullet, 5.0f,
                   20, blueprint2);

    REQUIRE_FALSE(weapon1 == weapon2);
  }

  SECTION("== returns false when reload_rate is different") {
    ProjectileBlueprint blueprint1(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon1("test", kBullet, 1.0f,
                   10, blueprint1);

    ProjectileBlueprint blueprint2(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon2("test", kBullet, 1.0f,
                   20, blueprint2);

    REQUIRE_FALSE(weapon1 == weapon2);
  }

  SECTION("== returns false when blueprint is different") {
    ProjectileBlueprint blueprint1(1.0f, 10, 1.0f,
                                   false, 0.0f);
    Weapon weapon1("test", kBullet, 1.0f,
                   10, blueprint1);

    ProjectileBlueprint blueprint2(1.0f, 12, 1.0f,
                                   false, 0.0f);
    Weapon weapon2("test", kBullet, 1.0f,
                   10, blueprint2);

    ProjectileBlueprint blueprint3(1.0f, 10, 1.0f,
                                   false, 1.0f);
    Weapon weapon3("test", kBullet, 1.0f,
                   10, blueprint2);

    REQUIRE_FALSE(weapon1 == weapon2);
    REQUIRE_FALSE(weapon1 == weapon3);
  }
}

TEST_CASE("Reload works accordingly to set last_fire") {
  ProjectileBlueprint blueprint1(1.0f, 10, 1.0f,
                                 false, 0.0f);
  Weapon weapon1("test", kBullet, 1.0f,
                 1000, blueprint1);

  std::chrono::milliseconds dura(1000);
  std::this_thread::sleep_for(dura); // pause code for 1 second to load

  std::chrono::milliseconds time_difference =
      std::chrono::duration_cast<std::chrono::milliseconds>(weapon1.get_last_fire_() -
                                               std::chrono::system_clock::now());

  REQUIRE(time_difference.count()== -1000); // last_fire is 1000 milliseconds ago

  weapon1.Reload();
  time_difference =
      std::chrono::duration_cast<std::chrono::milliseconds>(weapon1.get_last_fire_() -
                                              std::chrono::system_clock::now());

  REQUIRE(time_difference.count() == 0); // begin_time is reset
}