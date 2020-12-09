#include "core/enemy_spawner.h"

namespace shooter {

EnemySpawner::EnemySpawner(glm::ivec2 board_dimensions) :
    spawns_(){
  CreateSpawns(board_dimensions);
}

void EnemySpawner::CreateSpawns(glm::ivec2 board_dimensions) {
  int x_unit = static_cast<int>(board_dimensions.x) / kSpawnDivision;
  int y_unit = static_cast<int>(board_dimensions.y) / kSpawnDivision;

  for (int x_coord = 0; x_coord < board_dimensions.x + x_unit;
       x_coord += x_unit) {
    spawns_.emplace_back(x_coord, 0);
    spawns_.emplace_back(x_coord, board_dimensions.y);
  }

  for (int y_coord = y_unit; y_coord < board_dimensions.y;
       y_coord += y_unit) {
    spawns_.emplace_back(0, y_coord);
    spawns_.emplace_back(board_dimensions.x, y_coord);
  }

}

const std::vector<glm::ivec2>& EnemySpawner::get_spawns_() const {
  return spawns_;
}

std::vector<Enemy> EnemySpawner::SpawnEnemies(long time_from_start,
                                              long time_from_last_wave) const {
  std::vector<Enemy> enemies;

  if (time_from_last_wave <= kEnemyWaveSpawnDuration) { // not yet time to spawn enemies
    return enemies;
  }

  // spawns an additional enemy per spawn every 60 seconds
  size_t num_enemy_spawn = 1 + static_cast<size_t>(time_from_start) / kAdditionalEnemyDuration;
  // difficulty increases every 20 seconds ( higher health, faster speed for enemy)
  size_t difficulty = 1 + static_cast<size_t>(time_from_start) / kEnemyDifficultyDuration;

  if (difficulty > 5) {
    difficulty = 5; // max difficulty at 5
  }

  for (num_enemy_spawn; num_enemy_spawn != 0; num_enemy_spawn--) {
    enemies.push_back(CreateEnemy(difficulty, 20.0f, 50, 0.05f));
    enemies.push_back(CreateEnemy(difficulty, 0.0f, 2, 0.1f));
  }

  return enemies;
}

Enemy EnemySpawner::CreateEnemy(size_t difficulty,
                                float radius_scale,
                                int health_scale,
                                float level_scale) const {

  size_t index = (rand() % spawns_.size());
  float radius = (10.0f + static_cast<float>(rand()) /
                          RAND_MAX * difficulty * radius_scale);
  size_t health = 1 + static_cast<int>(difficulty * (static_cast<float>(rand())
                                                     / RAND_MAX) * health_scale);
  float level = 0.2f + difficulty * (static_cast<float>(rand()) / RAND_MAX) *
                       level_scale;

  return Enemy(spawns_[index], radius, health, 10, level);
}

} // shooter