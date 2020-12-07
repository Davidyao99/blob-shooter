#include "core/projectile_blueprint.h"

namespace shooter {

  ProjectileBlueprint::ProjectileBlueprint(float projectile_radius, size_t damage,
                                         float speed, bool isExplosive,
                                         float explosion_radius) :
  projectile_radius_(projectile_radius),
      damage_(damage), speed_(speed), isExplosive_(isExplosive),
  explosion_radius_(explosion_radius){

  }

  bool ProjectileBlueprint::operator==(const ProjectileBlueprint rhs) const {
    return (projectile_radius_ == rhs.projectile_radius_ &&
            damage_ == rhs.damage_ && speed_ == rhs.speed_ &&
            isExplosive_ == rhs.isExplosive_ && explosion_radius_ == rhs.explosion_radius_);
  }

}