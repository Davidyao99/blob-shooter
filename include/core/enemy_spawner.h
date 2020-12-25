#pragma once

#include "cinder/gl/gl.h"
#include "core/enemy.h"
#include "core/game_config_constants.h"

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

  std::vector<Enemy> SpawnEnemies(long time_from_start,
                                  long time_from_last_wave) const;

  /**
   * get vector of enemy spawning locations
   * @return vector of positions
   */

  const std::vector<glm::ivec2>& get_spawns_() const;

 private:

  std::vector<glm::ivec2> spawns_;

  /**
   * Create spawning locations on the parameter of the board using board dimensions
   * 21 spawns on each side
   * @param board_dimensions
   */

  void CreateSpawns(glm::ivec2 board_dimensions);

  /**
   * Creates enemy with random attributes according
   * @param difficulty how difficult this enemy will be to deal with ( increases with time)
   * @param radius_scale how much radius will scale with difficulty
   * @param health_scale how much health will scale by difficulty
   * @param level_scale how much the acceleration will scale by difficulty
   * @return
   */

  Enemy CreateEnemy(size_t difficulty, float radius_scale, int health_scale,
                    float level_scale) const;


};

}  // namespace shooter
