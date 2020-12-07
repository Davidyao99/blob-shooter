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

    ProjectileBlueprint(float projectile_radius, size_t damage, float speed,
                        bool isExplosive, float explosion_radius);

    /**
     * == operator to be used for testing equality between ProjectileBlueprints
     * @param rhs
     * @return true if same, false otherwise
     */
    bool operator==(const ProjectileBlueprint rhs) const;

    float projectile_radius_;

    size_t damage_;

    float speed_;

    bool isExplosive_;

    float explosion_radius_;
  };
}
