#include <catch2/catch.hpp>
#include "core/player.h"
#include <chrono>
#include <thread>

using shooter::Player;
using shooter::Direction;

TEST_CASE("Player Accelerate works accordingly") {
  Player player(glm::vec2(0,5), 1.0f, 2);

  SECTION("Accelerate works for left") {
    player.Accelerate(Direction::left);
    REQUIRE(player.get_velocity_() == glm::vec2(-1,0));
  }

  SECTION("Accelerate works for right") {
    player.Accelerate(Direction::right);
    REQUIRE(player.get_velocity_() == glm::vec2(1,0));
  }

  SECTION("Accelerate works for up") {
    player.Accelerate(Direction::up);
    REQUIRE(player.get_velocity_() == glm::vec2(0,-1));
  }

  SECTION("Accelerate works for down") {
    player.Accelerate(Direction::down);
    REQUIRE(player.get_velocity_() == glm::vec2(0,1));
  }
}

TEST_CASE("Player Move works accordingly") {
  Player player(glm::vec2(0,5), 1.0f,2);
  player.Accelerate(Direction::right);

  SECTION("Player Move adds velocity to position") {
    player.Move();
    REQUIRE(player.get_position_() == glm::vec2(1,5));
  }

  SECTION("Player Move gradually reduces velocity") {
    player.Move();
    REQUIRE(player.get_velocity_() == glm::vec2(0.9,0));
  }

  SECTION("Player Move rounds to zero after reptitive calls without accelerating") {
    for (size_t iter = 30; iter != 0; iter --) {
      player.Move();
    }
    REQUIRE(player.get_velocity_() == glm::vec2(0,0));
  }

}

TEST_CASE("Player Shoot works accordingly") {

  Player player(glm::vec2(0,5), 1.0f,2);

  SECTION("Returns true if last_fire >= 1 second ago") {
    player.Shoot();
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura); // pause code for 1 second
    REQUIRE(player.Shoot());
  }

  SECTION("Returns false if last_fire < 1 second ago") {
    player.Shoot();
    std::chrono::milliseconds dura(500);
    std::this_thread::sleep_for(dura); // pause code for 1 second
    REQUIRE_FALSE(player.Shoot());
  }
}

TEST_CASE("Player GetReloadStatus works accordingly") {

  Player player(glm::vec2(0,5), 1.0f,2);

  SECTION("Returns percentage of reload time correctly") {
    player.Shoot();
    std::chrono::milliseconds dura1(500);
    std::this_thread::sleep_for(dura1); // pause code for 0.5 second
    float reload_status1 = player.GetReloadStatus();
    std::chrono::milliseconds dura2(250);
    std::this_thread::sleep_for(dura2); // pause code for 0.25 second
    float reload_status2 = player.GetReloadStatus();
    REQUIRE(0.5f == Approx(reload_status1).epsilon(1e-2));
    REQUIRE(0.75f == Approx(reload_status2).epsilon(1e-2));
  }

  SECTION("Returns 1.0 when duration from last fire > 1.0 sec") {
    player.Shoot();
    std::chrono::milliseconds dura(1200);
    std::this_thread::sleep_for(dura); // pause code for 1.2 second
    float reload_status = player.GetReloadStatus();
    REQUIRE(1.0f == Approx(reload_status).epsilon(1e-2));
  }
}

TEST_CASE("ZeroXVelocity works accordingly and zeros x velocity") {
  Player player(glm::vec2(0,5), 1.0f,2);
  player.Accelerate(Direction::right);
  player.Accelerate(Direction::down);
  player.ZeroXVelocity();
  REQUIRE(glm::vec2(0,1) == player.get_velocity_());
}

TEST_CASE("ZeroYVelocity works accordingly and zeros x velocity") {
  Player player(glm::vec2(0,5), 1.0f,2);
  player.Accelerate(Direction::right);
  player.Accelerate(Direction::down);
  player.ZeroYVelocity();
  REQUIRE(glm::vec2(1,0) == player.get_velocity_());
}
