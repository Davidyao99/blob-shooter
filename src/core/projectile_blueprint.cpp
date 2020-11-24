#include "core/projectile_blueprint.h"

namespace shooter {

  ProjectileBlueprint::ProjectileBlueprint(float radius, int damage,
                                         float speed, bool isExplosive) :
  radius_(radius),
      damage_(damage), speed_(speed), isExplosive_(isExplosive){

}
}