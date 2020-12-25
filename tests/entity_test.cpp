#include <catch2/catch.hpp>
#include "core/entity.h"

using shooter::Entity;

TEST_CASE("Move method works accordingly") {
  Entity entity(glm::vec2 (10.0f, 10.0f), 10.0f, 10,
                glm::vec2 (1.0f, 1.0f));
  REQUIRE(entity.get_position_() == glm::vec2(10.0f, 10.0f));
  entity.Move();
  REQUIRE(entity.get_position_() == glm::vec2(11.0f, 11.0f));
}

TEST_CASE("IsDead method works accordingly") {

  SECTION("IsDead method returns true when hit_points is 0") {
    Entity entity(glm::vec2(10.0f, 10.0f), 10.0f, 0,
                  glm::vec2 (1.0f, 1.0f));
    REQUIRE(entity.IsDead());
  }

  SECTION("IsDead method returns true when hit_points is negative") {
    Entity entity(glm::vec2(10.0f, 10.0f), 10.0f, -5,
                  glm::vec2 (1.0f, 1.0f));
    REQUIRE(entity.IsDead());
  }

  SECTION("IsDead method returns false when hit_points it position") {
    Entity entity(glm::vec2(10.0f, 10.0f), 10.0f, 6,
                  glm::vec2 (1.0f, 1.0f));
    REQUIRE_FALSE(entity.IsDead());
  }

}

TEST_CASE("Collide works accordingly") {

  Entity entity1(glm::vec2(0,5), 2.0f,10,glm::vec2(0,0));
  Entity entity2(glm::vec2(0,2), 1.0f, 2,
                glm::vec2(0,7));
  entity1.Collide(entity2);

  SECTION("Collision with bullet reduces hit points on enemy") {
    REQUIRE(entity1.get_hit_points_() == 8);
  }

  SECTION("Collision with bullet changes velocity of enemy") {
    REQUIRE(entity1.get_velocity_() == glm::vec2(0,2));
  }

  SECTION("Collision with bullet reduces hit points on bullet") {
    REQUIRE(entity2.get_hit_points_() == -8);
  }
}
