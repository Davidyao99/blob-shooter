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
using shooter::ProjectileType;

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
    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false, 0.0f);
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
    engine.AddEnemy(Enemy(glm::vec2(5,25), 1.0f, 1, 1, 1.0f));
    engine.AddEnemy(Enemy(glm::vec2(45,25), 1.0f, 1, 1, 1.0f));
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
    engine.AddEnemy(Enemy(glm::vec2(0,25), 1.0f, 0, 1, 0.01f));
    engine.AddEnemy(Enemy(glm::vec2(50,25), 1.0f, 0, 1, 0.01f));
    engine.AddEnemy(Enemy(glm::vec2(45,25), 1.0f, 2, 1, 1.0f));
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

    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false, 0.0f);

    SECTION("Handle Collision works between enemies and bullets"){
      engine.AddBullet(Bullet(glm::vec2(25,25),
                       blueprint, glm::vec2(26,25)));
      engine.AddEnemy(Enemy(glm::vec2(26,25),
                      1.0f, 20, 2, 0.3f));
      engine.HandleCollisions();
      REQUIRE(engine.get_enemies_().size() == 1);
      REQUIRE(engine.get_bullets_().size() == 1);
      REQUIRE(engine.get_enemies_().at(0).get_health_() == 10);
      REQUIRE(engine.get_bullets_().at(0).get_health_() == -1);
      // enemy gets knocked back
      REQUIRE(engine.get_enemies_().at(0).get_velocity_() == glm::vec2(10,0));
    }

    SECTION("Handle Collision works between enemies and player"){
      engine.AddEnemy(Enemy(glm::vec2(0,1),
                      1.0f, 20, 2, 0.3f));
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
    engine.HandleShoot(glm::vec2(2,0));
    REQUIRE(engine.get_bullets_().size() == 1);
    REQUIRE(engine.get_bullets_().at(0).get_velocity_() == glm::vec2 (10,0));
  }

  SECTION("Handle Shoot does not spawn bullet when duration from last shot < "
      "current weapon") {
    std::chrono::milliseconds dura(250);
    std::this_thread::sleep_for(dura); // pause code for 0.25 seconds
    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 0);
    std::this_thread::sleep_for(dura); // pause code for another 0.25 seconds
    std::this_thread::sleep_for(dura); // pause code for another 0.25 seconds
    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 0);
    std::this_thread::sleep_for(dura); // pause code for another 0.25 seconds
    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 1);
  }

  SECTION("Handle Shoot returns the right projectile type when fired") {
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura); // pause code for 1 second to load
    ProjectileType projectile_type_1 = engine.HandleShoot(glm::vec2(2,0));
    engine.ChangeWeapon(false);
    engine.ChangeWeapon(false); // change to laser
    ProjectileType projectile_type_2 = engine.HandleShoot(glm::vec2(2,0));
    engine.ChangeWeapon(false);
    ProjectileType projectile_type_3 = engine.HandleShoot(glm::vec2(2,0));
    REQUIRE(projectile_type_1 == shooter::bullet);
    REQUIRE(projectile_type_2 == shooter::beam);
    REQUIRE(projectile_type_3 == shooter::bullet);
  }

  SECTION("Handle Shoot resets the weapon reload timing when fired") {
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura); // pause code for 1 second to load
    REQUIRE(engine.get_player_().GetWeaponReloadStatus() == 1.0f);
    engine.HandleShoot(glm::vec2(2,0));
    REQUIRE(engine.get_player_().GetWeaponReloadStatus() == 0.0f);
  }
}

TEST_CASE("ShootBeam works accordingly") {

  Engine engine(200,50, glm::vec2(100,25));

  SECTION("Hits multiple enemies that are in a straight line") {
    engine.AddEnemy(Enemy(glm::vec2(50,25),
                          1.0f, 20, 2, 0.3f));
    engine.AddEnemy(Enemy(glm::vec2(80,25),
                          1.0f, 30, 2, 0.3f));
    engine.AddEnemy(Enemy(glm::vec2(20,40),
                          1.0f, 40, 2, 0.3f));
    engine.ShootBeam(glm::vec2(-1,0), 1.0f, 10);
    REQUIRE(engine.get_enemies_().size() == 3);
    REQUIRE(engine.get_enemies_().at(0).get_health_() == 10);
    REQUIRE(engine.get_enemies_().at(1).get_health_() == 20);
    REQUIRE(engine.get_enemies_().at(2).get_health_() == 40);
  }

  SECTION("Hits enemies that are touching the edges of the beam") {
    engine.AddEnemy(Enemy(glm::vec2(150,26.9f),
                          1.0f, 20, 2, 0.3f));
    engine.AddEnemy(Enemy(glm::vec2(120,27.1f),
                          1.0f, 20, 2, 0.3f));
    engine.ShootBeam(glm::vec2(1,0), 1.0f, 10);
    REQUIRE(engine.get_enemies_().size() == 2);
    REQUIRE(engine.get_enemies_().at(0).get_health_() == 10);
    REQUIRE(engine.get_enemies_().at(1).get_health_() == 20);
  }

  SECTION("Enemies behind the player will not be hit") {
    engine.AddEnemy(Enemy(glm::vec2(50,25),
                          1.0f, 20, 2, 0.3f));
    engine.ShootBeam(glm::vec2(1,0), 1.0f, 10);
    REQUIRE(engine.get_enemies_().size() == 1);
    REQUIRE(engine.get_enemies_().at(0).get_health_() == 20);
  }
}

TEST_CASE("AddEnemy adds enemy to vector of enemies") {

  Engine engine(200,50, glm::vec2(100,25));

  Enemy enemy(glm::vec2(150,26.9f),
              1.0f, 20, 2, 0.3f);

  engine.AddEnemy(enemy);

  REQUIRE(engine.get_enemies_().size() == 1);
  REQUIRE(engine.get_enemies_().at(0) == enemy);

}

TEST_CASE("AddBullet adds bullet to vector of bullets") {

  Engine engine(200,50, glm::vec2(100,25));

  ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false, 0.0f);
  Bullet bullet(glm::vec2(25,25),
                blueprint, glm::vec2(26,25));
  engine.AddBullet(bullet);

  REQUIRE(engine.get_bullets_().size() == 1);
  REQUIRE(engine.get_bullets_().at(0) == bullet);
}

TEST_CASE("ChangeWeapon works accordingly") {

  Engine engine(200,50, glm::vec2(100,25));

  SECTION("Changes weapon to next weapon when parameter set to true") {
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Pistol");
    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Sniper");
    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Rifle");
  }

  SECTION("Changes weapon to prev weapon when parameter set to false") {
    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Rifle");
    engine.ChangeWeapon(false);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Sniper");
    engine.ChangeWeapon(false);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Pistol");
  }

  SECTION("Change weapon to next weapon works on last weapon") {
    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Rocket");
    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Pistol");
    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Sniper");
  }

  SECTION("Change weapon to prev weapon works on first weapon") {
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Pistol");
    engine.ChangeWeapon(false);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Rocket");
    engine.ChangeWeapon(false);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Laser");
  }
}

TEST_CASE("Explode works accordingly") {

  Engine engine(200,50, glm::vec2(100,25));

  SECTION("Explode damages all enemies within radius") {
    engine.AddEnemy(
        Enemy(glm::vec2(51,25),
                    1.0f, 20, 2, 0.3f));
    engine.AddEnemy(
        Enemy(glm::vec2(50,35),
              1.0f, 20, 2, 0.3f));
    engine.AddEnemy(
        Enemy(glm::vec2(50,15),
              1.0f, 20, 2, 0.3f));
    engine.AddEnemy(
        Enemy(glm::vec2(50,10),
              1.0f, 20, 2, 0.3f));
    engine.Explode(glm::vec2(50,25), 10.0f, 5);
    REQUIRE(engine.get_enemies_().at(0).get_health_() == 15);
    REQUIRE(engine.get_enemies_().at(1).get_health_() == 15);
    REQUIRE(engine.get_enemies_().at(2).get_health_() == 15);
    REQUIRE(engine.get_enemies_().at(3).get_health_() == 20);
  }

  SECTION("Explode damages player for 1/4 of the damage") {
    REQUIRE(engine.get_player_().get_health_() == 50.0f);
    engine.Explode(glm::vec2(90,25), 10.0f, 40);
    REQUIRE(engine.get_player_().get_health_() == 40.0f);
  }

}