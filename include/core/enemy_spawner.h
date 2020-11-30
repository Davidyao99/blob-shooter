#pragma once

#include "cinder/gl/gl.h"
#include "core/enemy.h"

namespace shooter {

  class EnemySpawner {

   public:

    EnemySpawner(glm::ivec2);

    std::vector<Enemy> SpawnEnemy(long time_from_start,
                                  long time_from_last_wave) const;

    void CreateSpawns(glm::ivec2 board_dimensions);

    const std::vector<glm::ivec2>& get_spawns_() const;

   private:

    std::vector<glm::ivec2> spawns_;

   };
}
