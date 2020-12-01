#pragma once

#include "cinder/gl/gl.h"
#include "core/enemy.h"

namespace shooter {

  /**
   *  Class responsible for spawning enemies. Used by engine to spawn enemies
   */

  class EnemySpawner {

   public:

    /**
     * Constructor of EnemySpawner
     * @param board_dimensions dimensions of board, used to create enemy
     * spawns
     */

    EnemySpawner(glm::ivec2 board_dimensions);

    /**
     * SpawnEnemy creates a vector of enemy to be spawned
     * @param time_from_start time passed to decide on size and difficult of wave
     * to be spawned
     * @param time_from_last_wave time from last wave
     * @return vector of enemies
     */

    std::vector<Enemy> SpawnEnemy(long time_from_start,
                                  long time_from_last_wave) const; //Todo is there a better way

    /**
     * Create spawning locations on the parameter of the board using board dimensions
     * 21 spawns on each side
     * @param board_dimensions
     */

    void CreateSpawns(glm::ivec2 board_dimensions);

    /**
     * get vector of enemy spawning locations
     * @return vector of positions
     */

    const std::vector<glm::ivec2>& get_spawns_() const;

   private:

    std::vector<glm::ivec2> spawns_;

   };
}
