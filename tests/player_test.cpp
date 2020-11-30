#include <catch2/catch.hpp>
#include "core/player.h"
#include "core/engine.h"
#include <chrono>
#include <thread>

namespace shooter {

  TEST_CASE("Player Accelerate works accordingly") {
    Player player(glm::vec2(0, 5), 1.0f, 2);

    SECTION("Accelerate works for left") {
      player.Accelerate(Direction::left);
      REQUIRE(player.get_velocity_() == glm::vec2(-1, 0));
    }

    SECTION("Accelerate works for right") {
      player.Accelerate(Direction::right);
      REQUIRE(player.get_velocity_() == glm::vec2(1, 0));
    }

    SECTION("Accelerate works for up") {
      player.Accelerate(Direction::up);
      REQUIRE(player.get_velocity_() == glm::vec2(0, -1));
    }

    SECTION("Accelerate works for down") {
      player.Accelerate(Direction::down);
      REQUIRE(player.get_velocity_() == glm::vec2(0, 1));
    }
  }

  TEST_CASE("Player Move works accordingly") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    player.Accelerate(Direction::right);

    SECTION("Player Move adds velocity to position") {
      player.Move();
      REQUIRE(player.get_position_() == glm::vec2(1, 5));
    }

    SECTION("Player Move gradually reduces velocity") {
      player.Move();
      REQUIRE(player.get_velocity_() == glm::vec2(0.9, 0));
    }

    SECTION(
        "Player Move rounds to zero after reptitive calls without accelerating") {
      for (size_t iter = 30; iter != 0; iter--) {
        player.Move();
      }
      REQUIRE(player.get_velocity_() == glm::vec2(0, 0));
    }
  }

  TEST_CASE("ZeroXVelocity works accordingly and zeros x velocity") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    player.Accelerate(Direction::right);
    player.Accelerate(Direction::down);
    player.ZeroXVelocity();
    REQUIRE(glm::vec2(0, 1) == player.get_velocity_());
  }

  TEST_CASE("ZeroYVelocity works accordingly and zeros x velocity") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    player.Accelerate(Direction::right);
    player.Accelerate(Direction::down);
    player.ZeroYVelocity();
    REQUIRE(glm::vec2(1, 0) == player.get_velocity_());
  }

  TEST_CASE("Reload Weapon sets last reload to now") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    player.AddWeapon(Weapon(std::string("Pistol"),
                            ProjectileType::bullet, 0.0f,
                            1000,
                            ProjectileBlueprint(10.0f, 1,
                                     10.0f, false, 0.0f)));
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura); // pause code for 1 second
    std::chrono::system_clock::time_point before_reload =
        player.GetCurrentWeapon().get_last_fire_();
    player.ReloadWeapon();
    std::chrono::system_clock::time_point after_reload =
      player.GetCurrentWeapon().get_last_fire_();
    std::chrono::seconds duration(std::chrono::duration_cast<std::chrono::seconds>(after_reload -
                                                          before_reload));
    REQUIRE(duration.count() == 1);
  }

  TEST_CASE("GetWeaponReloadStatus works accordingly") {

    SECTION("GetWeaponReloadStatus returns right reload status") {
      Player player(glm::vec2(0, 5), 1.0f, 2);
      player.AddWeapon(
          Weapon(std::string("Pistol"), ProjectileType::bullet, 0.0f, 1000,
                 ProjectileBlueprint(10.0f, 1, 10.0f, false, 0.0f)));
      REQUIRE(player.GetWeaponReloadStatus() == 0.0f);
      std::chrono::milliseconds dura(500);
      std::this_thread::sleep_for(dura);  // pause code for 0.5
      REQUIRE(player.GetWeaponReloadStatus() == 0.5f);
      std::this_thread::sleep_for(dura);  // pause code for 0.5
      REQUIRE(player.GetWeaponReloadStatus() == 1.0f);
    }

    SECTION("GetWeaponReloadStatus returns reload status as 1 once fully reloaded") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    player.AddWeapon(
        Weapon(std::string("Pistol"), ProjectileType::bullet,0.0f, 1000,
               ProjectileBlueprint(10.0f, 1, 10.0f, false, 0.0f)));
    REQUIRE(player.GetWeaponReloadStatus() == 0.0f);
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura);
    REQUIRE(player.GetWeaponReloadStatus() == 1.0f);
    std::this_thread::sleep_for(dura);
    REQUIRE(player.GetWeaponReloadStatus() == 1.0f); // still 1 after waiting > reload time
    }
  }

  TEST_CASE("FireBullet fires bullet according to projectile blueprint") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    ProjectileBlueprint blue_print(10.0f, 1, 10.0f, false, 0.0f);
    player.AddWeapon(
        Weapon(std::string("Pistol"), ProjectileType::bullet, 0.0f, 1000,
               blue_print));
    Bullet bullet1 = player.FireBullet(glm::vec2(2,0));
    Bullet bullet_test(glm::ivec2(0,5), blue_print, glm::ivec2(2,0));
    REQUIRE(bullet1 == bullet_test);
  }

  TEST_CASE("ChangeNextWeapon works accordingly") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    player.AddWeapon(
        Weapon(std::string("Pistol"), bullet, 0.0f, 1000,
                       ProjectileBlueprint(10.0f, 1, 10.0f,
                                           false, 0.0f)));
    player.AddWeapon(
        Weapon("Sniper", bullet, 0.0f, 400,
                      ProjectileBlueprint(10.0f, 100,
                                          30.0f, false, 0.0f)));
    player.AddWeapon(
        Weapon("Laser", beam, 0.0f, 300,
                     ProjectileBlueprint(5.0f, 10,
                                         0, false, 0.0f)));
    REQUIRE(player.GetCurrentWeapon().get_name_() == "Pistol");
    player.ChangeNextWeapon();
    REQUIRE(player.GetCurrentWeapon().get_name_() == "Sniper");
    player.ChangeNextWeapon();
    REQUIRE(player.GetCurrentWeapon().get_name_() == "Laser");
    player.ChangeNextWeapon();
    REQUIRE(player.GetCurrentWeapon().get_name_() == "Pistol");
  }

  TEST_CASE("ChangePrevWeapon works accordingly") {
    Player player(glm::vec2(0, 5), 1.0f, 2);
    player.AddWeapon(
        Weapon(std::string("Pistol"), bullet, 0.0f, 1000,
               ProjectileBlueprint(10.0f, 1, 10.0f,
                                   false, 0.0f)));
    player.AddWeapon(
        Weapon("Sniper", bullet, 0.0f, 400,
               ProjectileBlueprint(10.0f, 100,
                                   30.0f, false, 0.0f)));
    player.AddWeapon(
        Weapon("Laser", beam, 0.0f, 300,
               ProjectileBlueprint(5.0f, 10,
                                   0, false, 0.0f)));
    REQUIRE(player.GetCurrentWeapon().get_name_() == "Pistol");
    player.ChangePrevWeapon();
    REQUIRE(player.GetCurrentWeapon().get_name_() == "Laser");
    player.ChangePrevWeapon();
    REQUIRE(player.GetCurrentWeapon().get_name_() == "Sniper");
  }


}
