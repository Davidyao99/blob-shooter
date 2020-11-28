#include <catch2/catch.hpp>
#include "core/enemy.h"
#include "core/bullet.h"

using shooter::Enemy;
using shooter::Bullet;

TEST_CASE("Accelerate works accordingly") {

  SECTION("Accelerate changes velocity accordingly") {
    Enemy enemy(glm::vec2(0,5), 2.0f,10,1,0.2f);
    enemy.Accelerate(glm::vec2(0,2));
    REQUIRE(glm::vec2(0, -0.95f) == enemy.get_velocity_());
  }

}


