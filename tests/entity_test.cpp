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
