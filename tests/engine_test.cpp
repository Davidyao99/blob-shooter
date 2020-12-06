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
using shooter::Collides;

TEST_CASE("MoveAllEntities works accordingly") {

  Engine engine(50,50);

  SECTION("MoveAllEntities moves player in single direction") {
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right};
    engine.MoveAllEntities(directions);

    Entity entity_test(glm::vec2(26,25), 10.0f, 50,
                       10, glm::vec2(0.9,0));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("MoveAllEntities moves player accordingly to moves in multiple directions") {
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};
    engine.MoveAllEntities(directions);

    Entity entity_test(glm::vec2(26,26), 10.0f, 50,
                       10, glm::vec2(0.9,0.9));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("MoveAllEntities does not move player if both opposing directions are pressed") {
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::left};
    engine.MoveAllEntities(directions);

    Entity entity_test(glm::vec2(25,25), 10.0f, 50,
                       10, glm::vec2(0,0));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("MoveAllEntities moves all bullets accordingly") {
    std::set<Direction> directions;
    Engine engine(50,50, glm::vec2(25,25));
    ProjectileBlueprint blueprint(1.0f, 10, 1.0f,
                                  false, 0.0f);

    engine.AddBullet(Bullet(glm::vec2(0,0), blueprint,
                            glm::vec2(0,5)));
    engine.AddBullet(Bullet(glm::vec2(0,0), blueprint,
                            glm::vec2(5,0)));

    engine.update(directions);

    Entity bullet_test1(glm::vec2(0,1), 1.0f, 1,
                        10, glm::vec2(0,1));
    Entity bullet_test2(glm::vec2(1,0), 1.0f, 1,
                        10, glm::vec2(1,0));

    REQUIRE(engine.get_bullets_().size() == 2);
    REQUIRE((Entity)engine.get_bullets_().at(0) == bullet_test1);
    REQUIRE((Entity)engine.get_bullets_().at(1) == bullet_test2);
  }

  SECTION("MoveAllEntities moves all enemies accordingly") {
    std::set<Direction> directions;
    Engine engine(50,50, glm::vec2(25,25));

    engine.AddEnemy(Enemy(glm::vec2(5,25), 1.0f,
                          1, 1, 1.0f));
    engine.AddEnemy(Enemy(glm::vec2(45,25), 1.0f,
                          1, 1, 1.0f));

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
}

TEST_CASE("HandlePlayerAtBoundary works accordingly") {

  SECTION("HandlePlayerAtBoundary zeroes y axis velocity when player moving"
      " out of north boundary") {

    Engine engine(50,50, glm::vec2(25,1));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::up};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(26,0), 10.0f, 50,
                       10, glm::vec2(0.9,0));
    REQUIRE((Entity)player == entity_test);
  }

  SECTION("HandlePlayerAtBoundary does not zero y axis velocity when player at"
          "north boundary but moving away from boundary") {

    Engine engine(50,50, glm::vec2(25,0));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(26,1), 10.0f, 50,
                       10, glm::vec2(0.9,0.9));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("HandlePlayerAtBoundary zeroes y axis velocity when player moving"
          " out of south boundary") {
    Engine engine(50,50, glm::vec2(25,50));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(26,51), 10.0f, 50,
                       10, glm::vec2(0.9,0));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("HandlePlayerAtBoundary does not zero y axis velocity when player at"
      "south boundary but moving away from boundary") {
    Engine engine(50,50, glm::vec2(25,50));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::up};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(26,49), 10.0f, 50,
                       10, glm::vec2(0.9,-0.9));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("HandlePlayerAtBoundary zeroes x axis velocity when player moving"
          " out of east boundary") {
    Engine engine(50,50, glm::vec2(50,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(51,26), 10.0f, 50,
                       10, glm::vec2(0,0.9));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("HandlePlayerAtBoundary does not zero x axis velocity when player at"
          " east boundary and moving away from boundary") {
    Engine engine(50,50, glm::vec2(50,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::left, Direction::down};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(49,26), 10.0f, 50,
                       10, glm::vec2(-0.9,0.9));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("HandlePlayerAtBoundary zeroes x axis velocity when player moving"
          " out of west boundary") {
    Engine engine(50,50, glm::vec2(0,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::left, Direction::down};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(-1,26), 10.0f, 50,
                       10, glm::vec2(0,0.9));

    REQUIRE((Entity)player == entity_test);
  }

  SECTION("HandlePlayerAtBoundary does not zero x axis velocity when player at"
          " west boundary but moving away from boundary") {
    Engine engine(50,50, glm::vec2(0,25));
    const Player &player = engine.get_player_();
    std::set<Direction> directions{Direction::right, Direction::down};

    engine.MoveAllEntities(directions); // set player in motion
    engine.HandlePlayerAtBoundary();

    Entity entity_test(glm::vec2(1,26), 10.0f, 50,
                       10, glm::vec2(0.9,0.9));

    REQUIRE((Entity)player == entity_test);
  }
}

TEST_CASE("HandleDeaths works accordingly") {

  SECTION("HandleDeaths removes dead enemies") {
    Engine engine(50,50, glm::vec2(25,25));

    engine.AddEnemy(Enemy(glm::vec2(0,25), 1.0f, 0, 1, 0.01f));
    engine.AddEnemy(Enemy(glm::vec2(50,25), 1.0f, 0, 1, 0.01f));
    engine.AddEnemy(Enemy(glm::vec2(45,25), 1.0f, 2, 1, 1.0f));
    REQUIRE(engine.get_enemies_().size() == 3);

    std::set<Direction> directions;
    engine.update(directions);

    const std::vector<Enemy> &enemies = engine.get_enemies_();

    Entity enemy_test(glm::vec2(44.05,25), 1.0f, 2,
                        1, glm::vec2(-0.95,0));

    REQUIRE(enemies.size() == 1);
    REQUIRE(enemies.at(0) == enemy_test);
  }

  SECTION("HandleDeaths removes dead bullets") {
    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false, 0.0f);

    Engine engine(50,50, glm::vec2(25,25));

    engine.AddBullet(Bullet(glm::vec2(0,25),
                            blueprint, glm::vec2(1,0)));
    engine.AddBullet(Bullet(glm::vec2(50,25),
                            blueprint, glm::vec2(1,0)));
    engine.AddBullet(Bullet(glm::vec2(45,25),
                            blueprint, glm::vec2(1,0)));

    engine.AddEnemy(Enemy(glm::vec2(0,25), 1.0f, 1, 1, 0.01f));
    engine.AddEnemy(Enemy(glm::vec2(50,25), 1.0f, 1, 1, 0.01f));

    engine.HandleCollisions(); // collide enemies and bullets so bullets health becomes 0

    REQUIRE(engine.get_bullets_().size() == 3);

    engine.HandleDeaths();

    const std::vector<Bullet> &bullets = engine.get_bullets_();
    Entity bullet_test(glm::vec2(45,25), 1.0f, 1,
                      10, glm::vec2(1,0));

    REQUIRE(bullets.size() == 1);
    REQUIRE(bullets.at(0) == bullet_test);
  }

  SECTION("Explosive bullets explode during death") {
    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, true, 10.0f);

    Engine engine(50,50, glm::vec2(0,0));

    engine.AddBullet(Bullet(glm::vec2(25,25),
                            blueprint, glm::vec2(1,0)));

    engine.AddEnemy(Enemy(glm::vec2(18,25), 1.0f, 1, 1, 0.01f));
    engine.AddEnemy(Enemy(glm::vec2(25,25), 1.0f, 1, 1, 0.01f));

    engine.HandleCollisions(); // collide enemies and bullets so bullet health becomes 0

    REQUIRE(engine.get_bullets_().size() == 1);

    engine.HandleDeaths();

    const std::vector<Bullet> &bullets = engine.get_bullets_();

    REQUIRE(bullets.size() == 0);
    REQUIRE(engine.get_explosions_().size() == 1); // explosion added
    REQUIRE(engine.get_enemies_().size() == 0); // enemies killed by explosions
  }

  SECTION("Non Explosive bullet does not explode during death") {
    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false, 10.0f);

    Engine engine(50,50, glm::vec2(0,0));

    engine.AddBullet(Bullet(glm::vec2(25,25),
                            blueprint, glm::vec2(1,0)));

    engine.AddEnemy(Enemy(glm::vec2(18,25), 1.0f, 1, 1, 0.01f));
    engine.AddEnemy(Enemy(glm::vec2(25,25), 1.0f, 1, 1, 0.01f));

    REQUIRE(engine.get_bullets_().size() == 1);

    engine.HandleCollisions(); // collide enemies and bullets so bullet health becomes 0

    engine.HandleDeaths();

    const std::vector<Bullet> &bullets = engine.get_bullets_();

    REQUIRE(bullets.size() == 0);
    REQUIRE(engine.get_explosions_().size() == 0); // no explosion
    REQUIRE(engine.get_enemies_().size() == 1); // enemy not killed by explosion
  }

}

TEST_CASE("HandleCollision works accordingly") {

  Engine engine(50,50, glm::vec2(0,0));

  SECTION("HandleCollision checks for Bullet and Enemy Collision") {

    ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false, 0.0f);

    SECTION("HandleCollision works between enemies and bullets"){
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

    SECTION("HandleCollision works between enemies and player"){
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

  SECTION("Handle Shoot spawns bullet heading towards cursor") {
    Engine engine(50,50, glm::vec2(25,25));
    std::chrono::seconds dura(1);
    std::this_thread::sleep_for(dura); // pause code for 1 second to load
    engine.HandleShoot(glm::vec2(2,0));
    Entity bullet_test(glm::vec2(25,25), 10.0f, 1,
                        120, glm::vec2(30,0));
    REQUIRE(engine.get_bullets_().size() == 1);
    REQUIRE(engine.get_bullets_().at(0) == bullet_test);
  }

  SECTION("Handle Shoot does not spawn bullet when duration from last shot < "
      "current weapon") {
    Engine engine(50,50, glm::vec2(25,25));

    engine.HandleShoot(glm::vec2(27,25)); // fire once

    std::chrono::milliseconds dura(250);
    std::this_thread::sleep_for(dura); // pause code for 0.25 seconds

    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 1); // only one from first fire

    std::this_thread::sleep_for(dura); // pause code for another 0.25 seconds
    std::this_thread::sleep_for(dura); // pause code for another 0.25 seconds

    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 1);

    std::this_thread::sleep_for(dura); // pause code for another 0.25 seconds

    engine.HandleShoot(glm::vec2(27,25));
    REQUIRE(engine.get_bullets_().size() == 2);
  }

  SECTION("Handle Shoot returns the right projectile type when fired") {
    Engine engine(50,50, glm::vec2(25,25));

    engine.HandleShoot(glm::vec2(27,25)); // fire once

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
    Engine engine(50,50, glm::vec2(25,25));
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

    engine.ShootBeam(glm::vec2(-1,0), 10);

    REQUIRE(engine.get_enemies_().size() == 3);
    REQUIRE(engine.get_enemies_().at(0).get_health_() == 10);
    REQUIRE(engine.get_enemies_().at(1).get_health_() == 20);
    REQUIRE(engine.get_enemies_().at(2).get_health_() == 40);
  }

  SECTION("Hits enemies that are touching the edges of the beam") {
    engine.AddEnemy(Enemy(glm::vec2(150,26.0f),
                          1.0f, 20, 2, 0.3f));
    engine.AddEnemy(Enemy(glm::vec2(120,27.1f),
                          1.0f, 20, 2, 0.3f));

    engine.ShootBeam(glm::vec2(1,0), 10);

    REQUIRE(engine.get_enemies_().size() == 2);
    REQUIRE(engine.get_enemies_().at(0).get_health_() == 10);
    REQUIRE(engine.get_enemies_().at(1).get_health_() == 20);
  }

  SECTION("Enemies behind the player will not be hit") {
    engine.AddEnemy(Enemy(glm::vec2(50,25),
                          1.0f, 20, 2, 0.3f));

    engine.ShootBeam(glm::vec2(1,0), 10);

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

    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Sniper");

    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Machine Gun");

    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Rifle");

  }

  SECTION("Changes weapon to prev weapon when parameter set to false") {

    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Rifle");

    engine.ChangeWeapon(false);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Machine Gun");

    engine.ChangeWeapon(false);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Sniper");

  }

  SECTION("Change weapon to next weapon works on last weapon") {

    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);
    engine.ChangeWeapon(true);

    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Rocket");

    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Sniper");

    engine.ChangeWeapon(true);
    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Machine Gun");

  }

  SECTION("Change weapon to prev weapon works on first weapon") {

    REQUIRE(engine.get_player_().GetCurrentWeapon().get_name_() == "Sniper");

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

  SECTION("Explode damages player for 1/5 of the damage") {
    REQUIRE(engine.get_player_().get_health_() == 50.0f);

    engine.Explode(glm::vec2(90,25), 10.0f, 50);
    REQUIRE(engine.get_player_().get_health_() == 40.0f);
  }

}

TEST_CASE("Restart works accordingly") {

  Engine engine(200,50);

  std::set<Direction> directions{Direction::right, Direction::down};
  engine.update(directions);

  ProjectileBlueprint blueprint(1.0f, 10, 1.0f, false, 0.0f);
  engine.AddBullet(Bullet(glm::vec2(0,10), blueprint, glm::vec2(0,5)));
  engine.AddBullet(Bullet(glm::vec2(0,0), blueprint, glm::vec2(5,0)));

  engine.AddEnemy(Enemy(glm::vec2(5,25), 1.0f, 1, 1, 1.0f));
  engine.AddEnemy(Enemy(glm::vec2(45,25), 1.0f, 1, 1, 1.0f));

  engine.Restart();

  std::chrono::system_clock::time_point test_now = std::chrono::system_clock::now();
  REQUIRE(engine.get_player_().get_position_() == glm::vec2(100,25));
  REQUIRE(engine.get_player_().get_health_() == 50);
  REQUIRE(engine.get_enemies_().size() == 0);
  REQUIRE(engine.get_bullets_().size() == 0);
  REQUIRE(engine.get_score_() == 0);
  REQUIRE(engine.get_explosions_().size() == 0);

  std::chrono::milliseconds time_difference =
      std::chrono::duration_cast<std::chrono::milliseconds>(engine.get_begin_time_()
                                                            - test_now);
  REQUIRE(time_difference.count()==0); // begin_time is reset

  time_difference =
      std::chrono::duration_cast<std::chrono::milliseconds>(
          engine.get_last_enemy_wave_() - test_now);
  REQUIRE(time_difference.count()==0); // last_enemy_wave is reset
}

TEST_CASE("PlayerIsDead works accordingly") {

  SECTION("Returns true when player health == 0") {
    Engine engine(200, 50);
    engine.Explode(glm::vec2(100, 25), 1.0f, 250);
    REQUIRE(engine.get_player_().get_health_() == 0);
    REQUIRE(engine.PlayerIsDead());
  }

  SECTION("Returns true when player health < 0") {
    Engine engine(200, 50);
    engine.Explode(glm::vec2(100, 25), 1.0f, 300);
    REQUIRE(engine.get_player_().get_health_() <= 0);
    REQUIRE(engine.PlayerIsDead());
  }

  SECTION("Returns false when player health > 0") {
    Engine engine(200, 50);
    engine.Explode(glm::vec2(100, 25), 1.0f, 100);
    REQUIRE(engine.get_player_().get_health_() > 0);
    REQUIRE_FALSE(engine.PlayerIsDead());
  }
}

TEST_CASE("Collides work correctly in detecting collisions") {

  SECTION("Collides return true when two circles overlap") {
    REQUIRE(Collides(glm::vec2(0,0), glm::vec2(3,0), 2, 2));
    REQUIRE(Collides(glm::vec2(0,0), glm::vec2(2,2), 3, 2));
  }

  SECTION("Collides return true when two circles completely overlap") {
    REQUIRE(Collides(glm::vec2(0,0), glm::vec2(0,0), 2, 3));
  }

  SECTION("Collides return true when two circles edges touch") {
    REQUIRE(Collides(glm::vec2(0,0), glm::vec2(3,0), 2, 1));
  }

  SECTION("Collides return false when two circles do not touch") {
    REQUIRE_FALSE(Collides(glm::vec2(0,0), glm::vec2(5,0), 2, 1));
  }

}

TEST_CASE("IsOutOfBounds works accordingly") {

  Engine engine(200, 50);

  SECTION("IsOutOfBounds returns true if position is 500 pixels outside of map boundary"
      "in the x axis") {
    REQUIRE(engine.IsOutOfBounds(glm::vec2(800, 25)));
    REQUIRE(engine.IsOutOfBounds(glm::vec2(-600, 25)));
  }

  SECTION("IsOutOfBounds returns true if position is 500 pixels outside of map boundary"
      "in the y axis") {
    REQUIRE(engine.IsOutOfBounds(glm::vec2(100, 600)));
    REQUIRE(engine.IsOutOfBounds(glm::vec2(100, -600)));
  }

  SECTION("IsOutOfBounds returns true if position is 500 pixels outside of map boundary"
      "diagonally") {
    REQUIRE(engine.IsOutOfBounds(glm::vec2(-550, -550)));
    REQUIRE(engine.IsOutOfBounds(glm::vec2(700, 560)));

  }

  SECTION("IsOutOfBounds returns false when player is in bounds") {
    REQUIRE_FALSE(engine.IsOutOfBounds(glm::vec2(500, 25)));
    REQUIRE_FALSE(engine.IsOutOfBounds(glm::vec2(-200, 25)));
    REQUIRE_FALSE(engine.IsOutOfBounds(glm::vec2(100, 300)));
    REQUIRE_FALSE(engine.IsOutOfBounds(glm::vec2(400, 200)));
  }

}







