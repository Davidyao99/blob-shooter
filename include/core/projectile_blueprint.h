#pragma once

namespace shooter {

  /**
   * Projectile blueprint that is connected with each weapon. Used as a template
   * to create bullets of the right attributes for each weapon
   */

  struct ProjectileBlueprint {

    /**
     * Constructor for Projectile Blueprint
     * @param projectile_radius radius of projectile
     * @param damage damage of bullet, if isExplosive = true, damage of explosion instead
     * as damage of bullet will be automatically treated as 0
     * @param speed
     * @param isExplosive
     * @param explosion_radius
     */

    ProjectileBlueprint(float projectile_radius, int damage, float speed,
                        bool isExplosive, float explosion_radius);

    float projectile_radius_;
    int damage_;
    float speed_;
    bool isExplosive_;
    float explosion_radius_;
  };
}
