#include "core/enemy_spawner.h"

namespace shooter {

  EnemySpawner::EnemySpawner(glm::ivec2 board_dimensions) :
  spawns_(){
    CreateSpawns(board_dimensions);
  }

  void EnemySpawner::CreateSpawns(glm::ivec2 board_dimensions) {
    int x_unit = static_cast<int>(board_dimensions.x) / 20;
    int y_unit = static_cast<int>(board_dimensions.y) / 20;
    for (size_t x_coord = 0; x_coord != board_dimensions.x + x_unit;
         x_coord += x_unit) {
      spawns_.emplace_back(x_coord, 0);
      spawns_.emplace_back(x_coord, board_dimensions.y);
    }
    for (size_t y_coord = y_unit; y_coord != board_dimensions.y;
         y_coord += y_unit) {
      spawns_.emplace_back(0, y_coord);
      spawns_.emplace_back(board_dimensions.x, y_coord);
    }
  }

  const std::vector<glm::ivec2>& EnemySpawner::get_spawns_() const {
    return spawns_;
  }

  std::vector<Enemy> EnemySpawner::SpawnEnemy(long time_from_start,
                                long time_from_last_wave) const {
    std::vector<Enemy> enemies;
    if (time_from_last_wave > 5000) { // spawns enemy every 5 secs
    // spawns an additional enemy per spawn every 30 seconds
    size_t num_enemy_spawn = 1 + static_cast<size_t>(time_from_start) / 30000;
    // difficult increases every 20 seconds ( higher health, faster speed for enemy)
    size_t difficulty = 1 + static_cast<size_t>(time_from_start) / 20000;
    if (difficulty > 5) {
      difficulty = 5; // max difficulty at 5
    }
    for (num_enemy_spawn; num_enemy_spawn != 0; num_enemy_spawn--) {
      size_t index = (rand() % spawns_.size());
      size_t health = static_cast<int>(difficulty * (static_cast<float>(rand()) / RAND_MAX) * 20.0f);
      float level = 0.1f + difficulty * (static_cast<float>(rand()) / RAND_MAX) * 0.18f;
      enemies.push_back(Enemy(spawns_[index], 10.0f, health, 10, level));
      }
    }
    return enemies;
  }
}