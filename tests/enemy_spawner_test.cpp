#include <catch2/catch.hpp>
#include "core/enemy_spawner.h"


namespace shooter {

  TEST_CASE("EnemySpawner Constructor creates board_dimensions correctly") {
    EnemySpawner enemy_spawner (glm::ivec2(20, 20));
    REQUIRE(enemy_spawner.get_spawns_().size() == 80);
  }

  TEST_CASE("SpawnEnemies works accordingly") {

    EnemySpawner enemy_spawner (glm::ivec2(20, 20));

    SECTION("SpawnEnemies does not spawn waves when time_from_last_wave less than 5 secs") {
      std::vector<Enemy> enemies_test = enemy_spawner.SpawnEnemies(4000,
                                                                   4000);
      REQUIRE(enemies_test.size() == 0);
    }

  SECTION("SpawnEnemies spawn wave when time_from_last_wave more than 5 secs") {
    std::vector<Enemy> enemies_test = enemy_spawner.SpawnEnemies(2000,
                                                                 6000);
    REQUIRE(enemies_test.size() == 2);
  }

  SECTION("SpawnEnemies spawn with extra enemy every 60 seconds from start") {
    std::vector<Enemy> enemies_test = enemy_spawner.SpawnEnemies(70000,
                                                                 6000);
    REQUIRE(enemies_test.size() == 4); // extra 2 enemies

    enemies_test = enemy_spawner.SpawnEnemies(190000,6000);
    REQUIRE(enemies_test.size() == 8); // extra 6 enemies
  }
  }
}