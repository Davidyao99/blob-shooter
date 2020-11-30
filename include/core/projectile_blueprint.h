#pragma once

namespace shooter {

  struct ProjectileBlueprint {

    ProjectileBlueprint(float projectile_radius, int damage, float speed,
                        bool isExplosive, float explosion_radius);

    float projectile_radius_;
    int damage_;
    float speed_;
    bool isExplosive_;
    float explosion_radius_;
  };
}
