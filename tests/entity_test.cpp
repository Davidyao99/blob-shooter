#include <catch2/catch.hpp>
#include "core/entity.h"

using shooter::Entity;

TEST_CASE("Move method works accordingly") {
  Entity entity(glm::vec2 (10.0f, 10.0f), 10.0f, 10, 2,
                glm::vec2 (1.0f, 1.0f));
  REQUIRE(entity.get_position_() == glm::vec2(10.0f, 10.0f));
  entity.Move();
  REQUIRE(entity.get_position_() == glm::vec2(11.0f, 11.0f));
}

TEST_CASE("IsDead method works accordingly") {

  SECTION("IsDead method returns true when hit_points is 0") {
    Entity entity(glm::vec2(10.0f, 10.0f), 10.0f, 0,
                  2,glm::vec2 (1.0f, 1.0f));
    REQUIRE(entity.IsDead());
  }

  SECTION("IsDead method returns true when hit_points is negative") {
    Entity entity(glm::vec2(10.0f, 10.0f), 10.0f, -5,
                  2,glm::vec2 (1.0f, 1.0f));
    REQUIRE(entity.IsDead());
  }

  SECTION("IsDead method returns false when hit_points is positive") {
    Entity entity(glm::vec2(10.0f, 10.0f), 10.0f, 6,
                  2,glm::vec2 (1.0f, 1.0f));
    REQUIRE_FALSE(entity.IsDead());
  }
}

TEST_CASE("Collide works accordingly") {

  Entity entity1(glm::vec2(0,5), 2.0f,10,
                 3,glm::vec2(0,0));
  Entity entity2(glm::vec2(0,2), 1.0f, 5,
                2,glm::vec2(0,7));
  entity1.Collide(entity2);

  SECTION("Collision reduces hit points on both") {
    REQUIRE(entity1.get_health_() == 8);
    REQUIRE(entity2.get_health_() == 2);
  }

  SECTION("Collision changes velocity based on damage received") {
    REQUIRE(entity1.get_velocity_() == glm::vec2(0,2));
    REQUIRE(entity2.get_velocity_() == glm::vec2(0,4));
  }
}

TEST_CASE("Hit works accordingly") {

  Entity entity1(glm::vec2(0,5), 2.0f,10,
                 3,glm::vec2(0,0));
  entity1.Hit(4, glm::vec2(0,0));

  SECTION("Hit reduces health accordingly") {
    REQUIRE(entity1.get_health_() == 6);
  }

  SECTION("Rebound velocity added accordingly") {
    REQUIRE(entity1.get_velocity_() == glm::vec2(0, 4));
  }

}
