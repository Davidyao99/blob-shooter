#include "core/projectile_blueprint.h"

namespace shooter {

  ProjectileBlueprint::ProjectileBlueprint(float projectile_radius, int damage,
                                         float speed, bool isExplosive,
                                         float explosion_radius) :
  projectile_radius_(projectile_radius),
      damage_(damage), speed_(speed), isExplosive_(isExplosive),
  explosion_radius_(explosion_radius){

}
}