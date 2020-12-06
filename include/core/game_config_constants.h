#pragma once

#include "core/weapon.h"

namespace shooter {

  // all time are in milliseconds

  extern const int kScreenLength;
  extern const int kScreenHeight;
  extern const int kMapSize; // size of map relative to screen
  extern const int kSpawnDivision; // how many divisions per side, for creating spawns and drawing grid

  extern const int kEnemyWaveSpawnDuration; // time interval between new wave
  extern const int kAdditionalEnemyDuration; // time interval when an additional enemy is spawned every wave
  extern const int kEnemyDifficultyDuration; // time interval when difficulty increases (enemy spawned are better)

  extern const Weapon kSniper;
  extern const Weapon kMachineGun;
  extern const Weapon kRifle;
  extern const Weapon kLaser;
  extern const Weapon kRocket;
}
