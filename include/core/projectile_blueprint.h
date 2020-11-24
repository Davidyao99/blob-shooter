#pragma once

namespace shooter {

  struct ProjectileBlueprint {

    ProjectileBlueprint(float radius, int damage, float speed,
                        bool isExplosive);

    float radius_;
    int damage_;
    float speed_;
    bool isExplosive_;
  };
}
