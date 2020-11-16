#include <catch2/catch.hpp>
#include "core/engine.h"
#include "core/player.h"
#include "core/bullet.h"
#include <chrono>
#include <thread>

using shooter::Engine;
using shooter::Player;
using shooter::Bullet;
using shooter::Direction;
using shooter::Enemy;

TEST_CASE("Engine Update works accordingly") {

  SECTION("Update moves player accordingly to moves") {
    Engine engine(50,50);
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};
    engine.update(directions);
    REQUIRE(player.get_velocity() == glm::vec2(0.9,0.9));
    REQUIRE(player.get_position_() == glm::vec2(26,26));
  }

  SECTION("Update moves player and prevents moving out of north boundary") {
    Engine engine(50,50, glm::vec2(25,0));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::up};
    engine.update(directions);
    REQUIRE(player.get_velocity() == glm::vec2(0.9,0));
    REQUIRE(player.get_position_() == glm::vec2(26,0));
  }

  SECTION("Update moves player and prevents moving out of south boundary") {
    Engine engine(50,50, glm::vec2(25,50));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};
    engine.update(directions);
    REQUIRE(player.get_velocity() == glm::vec2(0.9,0));
    REQUIRE(player.get_position_() == glm::vec2(26,50));
  }

  SECTION("Update moves player and prevents moving out of east boundary") {
    Engine engine(50,50, glm::vec2(50,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};
    engine.update(directions);
    REQUIRE(player.get_velocity() == glm::vec2(0,0.9));
    REQUIRE(player.get_position_() == glm::vec2(50,26));
  }

  SECTION("Update moves player and prevents moving out of west boundary") {
    Engine engine(50,50, glm::vec2(0,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::left, Direction::down};
    engine.update(directions);
    REQUIRE(player.get_velocity() == glm::vec2(0,0.9));
    REQUIRE(player.get_position_() == glm::vec2(0,26));
  }

  SECTION("Update moves all bullets") {
    Engine engine(50,50, glm::vec2(25,25));
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura); // pause code for 1 second to fire
    engine.HandleShoot(glm::vec2(40,25));
    std::this_thread::sleep_for(dura); // pause code for 1 second to fire
    engine.HandleShoot(glm::vec2(25,26));
    std::set<Direction> directions;
    engine.update(directions);
    std::vector<Bullet> bullets = engine.get_bullets_();
    REQUIRE(bullets.size() == 2);
    REQUIRE(bullets.at(0).get_velocity_() == glm::vec2(10,0));
    REQUIRE(bullets.at(0).get_position_() == glm::vec2(35,25));
    REQUIRE(bullets.at(1).get_velocity_() == glm::vec2(0,10));
    REQUIRE(bullets.at(1).get_position_() == glm::vec2(25,35));
  }

  SECTION("Update moves all enemies") {
    std::set<Direction> directions;
    Engine engine(50,50, glm::vec2(25,25));
    engine.AddEnemy(glm::vec2(0,25), 1.0, 1, 1);
    engine.AddEnemy(glm::vec2(50,25), 1.0, 1, 1);
    engine.update(directions);
    const std::vector<Enemy> &enemies = engine.get_enemies_();
    REQUIRE(enemies.size() == 2);
    REQUIRE(enemies.at(0).get_position_() == glm::vec2(0.95,25));
    REQUIRE(enemies.at(1).get_position_() == glm::vec2(49.05, 25));
  }

  SECTION("Update spawns enemy every 10 seconds") {
    std::set<Direction> directions;
    Engine engine(50,50, glm::vec2(25,25));
    engine.update(directions);
    REQUIRE(engine.get_enemies_().size() == 0);
    std::chrono::seconds dura(11);
    std::this_thread::sleep_for(dura); // pause code for 10 second to spawn enemy
    engine.update(directions);
    REQUIRE(engine.get_enemies_().size() == 1);
    std::this_thread::sleep_for(dura); // pause code for 10 second to spawn more enemy
    engine.update(directions);
    REQUIRE(engine.get_enemies_().size() == 3);
  }
}

