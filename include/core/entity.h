#pragma once

#include "cinder/gl/gl.h"

namespace shooter{

/**
     * Parent class for enemy, player and bullet. Basic object with
     * common methods such as moving, collision, isDead and common
     * variables such as position, velocity and radius
     */

class Entity{

 public:

  /**
   * Testing constructor for entity
   * @param position position of entity
   * @param radius radius of entity
   * @param health health of entity
   * @param damage damage of entity
   * @param velocity velocity of entity
   */

  Entity(glm::vec2 position, float radius, int health, int damage, glm::vec2 velocity);

  /**
   * Constructor for entity
   * @param position position of entity
   * @param radius radius of entity
   * @param health health of entity
   * @param damage damage of entity
   */

  Entity(glm::vec2 position, float radius, int health, int damage);

  // basic getters
  const float get_radius_() const;

  const glm::vec2 get_position_() const;

  const glm::vec2 get_velocity_() const;

  const int get_health_() const;

  const size_t get_damage_() const;

  /**
   * Move method that adds velocity to position to move entity
   */

  virtual void Move();

  /**
   * Collides entity with other entity. calls hit() on both entities
   * @param entity other entity that is colliding into this entity
   */

  void Collide(Entity& entity);

  /**
   * boolean to check if hitpoints <= 0
   * @return true if hitpoints <= 0, false otherwise
   */

  bool IsDead() const;

  /**
   * Minus hit_point from entity health and adds rebound velocity
   * to entity velocity
   * @param hit_point amount to deduct from entity health
   * @param impact_position position of impact to calculate rebound velocity
   */

  void Hit(int hit_point, glm::vec2 impact_position);

  /**
   * overloaded == operator to be used for testing
   * @param other
   * @return true if member variables of other and this are the same
   */

  bool operator==(const Entity& other) const;

 protected:

  glm::vec2 position_;
  glm::vec2 velocity_;
  float radius_;
  int health_;
  size_t damage_;
};

} // namespace shooter