#include <catch2/catch.hpp>
#include "core/engine.h"
#include "core/player.h"
#include "core/bullet.h"
#include "core/projectile_blueprint.h"
#include <chrono>
#include <thread>

using shooter::Engine;
using shooter::Entity;
using shooter::Player;
using shooter::Bullet;
using shooter::Direction;
using shooter::Enemy;
using shooter::ProjectileBlueprint;

TEST_CASE("Engine constructor creates enemy spawns on boundaries") {
  Engine engine(40,40);
  std::vector<glm::vec2> spawns = engine.get_enemy_spawns_();
  for (auto &spawn: spawns) {
    std::cout<<"("<<spawn.x<<","<<spawn.y<<")"<<std::endl; // check visually
  }
  REQUIRE(engine.get_enemy_spawns_().size() == 80);
}

TEST_CASE("Engine Update works accordingly") {

  SECTION("Update moves player accordingly to moves") {
    Engine engine(50,50);
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};
    engine.update(directions);
    Entity entity_test(glm::vec2(26,26), 10.0f, 50,
                       10, glm::vec2(0.9,0.9));
    REQUIRE((Entity)player == entity_test);
  }

  SECTION("Update moves player and prevents moving out of north boundary") {
    Engine engine(50,50, glm::vec2(25,0));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::up};
    engine.update(directions);
    Entity entity_test(glm::vec2(26,0), 10.0f, 50,
                       10, glm::vec2(0.9,0));
    REQUIRE((Entity)player == entity_test);
  }

  SECTION("Update moves player and prevents moving out of south boundary") {
    Engine engine(50,50, glm::vec2(25,50));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};
    engine.update(directions);
    Entity entity_test(glm::vec2(26,50), 10.0f, 50,
                       10, glm::vec2(0.9,0));
    REQUIRE((Entity)player == entity_test);
  }

  SECTION("Update moves player and prevents moving out of east boundary") {
    Engine engine(50,50, glm::vec2(50,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};
    engine.update(directions);
    Entity entity_test(glm::vec2(50,26), 10.0f, 50,
                       10, glm::vec2(0,0.9));
    REQUIRE((Entity)player == entity_test);
  }

  SECTION("Update moves player and prevents moving out of west boundary") {
    Engine engine(50,50, glm::vec2(0,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::left, Direction::down};
    engine.update(directions);
    Entity entity_test(glm::vec2(0,26), 10.0f, 50,
                       10, glm::vec2(0,0.9));
    REQUIRE((Entity)player == entity_test);
  }

  SECTION("Update moves all bullets") {
    std::set<Direction> directions;
    Engine engine(50,50, glm::vec2(25,25));
    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false);
    engine.AddBullet(Bullet(glm::vec2(0,0), blueprint, glm::vec2(0,5)));
    engine.AddBullet(Bullet(glm::vec2(0,0), blueprint, glm::vec2(5,0)));
    engine.update(directions);
    Entity bullet_test1(glm::vec2(0,1), 1.0f, 1,
                       10, glm::vec2(0,1));
    Entity bullet_test2(glm::vec2(1,0), 1.0f, 1,
                        10, glm::vec2(1,0));
    REQUIRE(engine.get_bullets_().size() == 2);
    REQUIRE((Entity)engine.get_bullets_().at(0) == bullet_test1);
    REQUIRE((Entity)engine.get_bullets_().at(1) == bullet_test2);
  }

  SECTION("Update moves all enemies") {
    std::set<Direction> directions;
    Engine engine(50,50, glm::vec2(25,25));
    engine.AddEnemy(glm::vec2(5,25), 1.0f, 1, 1, 1.0f);
    engine.AddEnemy(glm::vec2(45,25), 1.0f, 1, 1, 1.0f);
    engine.update(directions);
    const std::vector<Enemy> &enemies = engine.get_enemies_();
    Entity enemy_test1(glm::vec2(5.95,25), 1.0f, 1,
                      1, glm::vec2(0.95,0));
    Entity enemy_test2(glm::vec2(44.05,25), 1.0f, 1,
                      1, glm::vec2(-0.95,0));
    REQUIRE(enemies.size() == 2);

    REQUIRE((Entity)enemies.at(0) == enemy_test1);
    REQUIRE((Entity)enemies.at(1) == enemy_test2);
  }

  SECTION("Update removes dead enemies") {
    std::set<Direction> directions;
    Engine engine(50,50, glm::vec2(25,25));
    engine.AddEnemy(glm::vec2(0,25), 1.0f, 0, 1, 0.01f);
    engine.AddEnemy(glm::vec2(50,25), 1.0f, 0, 1, 0.01f);
    engine.AddEnemy(glm::vec2(45,25), 1.0f, 2, 1, 1.0f);
    REQUIRE(engine.get_enemies_().size() == 3);
    engine.update(directions);
    const std::vector<Enemy> &enemies = engine.get_enemies_();
    Entity enemy_test(glm::vec2(44.05,25), 1.0f, 2,
                        1, glm::vec2(-0.95,0));
    REQUIRE(enemies.size() == 1);
    REQUIRE(enemies.at(0) == enemy_test);
  }
}

TEST_CASE("Handle Collision works accordingly") {

  Engine engine(50,50, glm::vec2(0,0));

  SECTION("Handle Collision checks for Bullet and Enemy Collision") {

    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false);

    SECTION("Handle Collision works between enemies and bullets"){
      engine.AddBullet(Bullet(glm::vec2(25,25),
                       blueprint, glm::vec2(26,25)));
      engine.AddEnemy(glm::vec2(26,25),
                      1.0f, 20, 2, 0.3f);
      engine.HandleCollisions();
      REQUIRE(engine.get_enemies_().size() == 1);
      REQUIRE(engine.get_bullets_().size() == 1);
      REQUIRE(engine.get_enemies_().at(0).get_health_() == 10);
      REQUIRE(engine.get_bullets_().at(0).get_health_() == -1);
      // enemy gets knocked back
      REQUIRE(engine.get_enemies_().at(0).get_velocity_() == glm::vec2(10,0));
    }

    SECTION("Handle Collision works between enemies and player"){
      engine.AddEnemy(glm::vec2(0,1),
                      1.0f, 20, 2, 0.3f);
      engine.HandleCollisions();
      REQUIRE(engine.get_enemies_().size() == 1);
      REQUIRE(engine.get_enemies_().at(0).get_health_() == 10);
      // enemy and player gets knocked back
      REQUIRE(engine.get_enemies_().at(0).get_velocity_() == glm::vec2(0,10));
      REQUIRE(engine.get_player_().get_velocity_() == glm::vec2(0,-2));
    }
  }
}

TEST_CASE("Handle Shoot works accordingly") {

  Engine engine(50,50, glm::vec2(25,25));

  SECTION("Handle Shoot spawns bullet heading towards cursor") {
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura); // pause code for 1 second to load
    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 1);
    REQUIRE(engine.get_bullets_().at(0).get_velocity_() == glm::vec2 (10,0));
  }

  SECTION("Handle Shoot does not spawn bullet when duration from last shot < 1sec") {
    std::chrono::milliseconds dura(250);
    std::this_thread::sleep_for(dura); // pause code for 0.25 seconds
    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 0);
    std::this_thread::sleep_for(dura); // pause code for 0.25 seconds
    std::this_thread::sleep_for(dura); // pause code for 0.25 seconds
    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 0);
  }
}



