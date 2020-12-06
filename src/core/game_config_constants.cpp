#include "core/game_config_constants.h"

namespace shooter {

  // all time are in milliseconds

  extern const int kScreenLength = 1200;
  extern const int kScreenHeight = 1000;
  extern const int kMapSize = 1; // size of map relative to screen
  extern const int kSpawnDivision = 17; // how many divisions per side, for creating spawns and drawing grid

  extern const int kEnemyWaveSpawnDuration = 5000; // time interval between new wave
  extern const int kAdditionalEnemyDuration = 60000; // time interval when an additional enemy is spawned every wave
  extern const int kEnemyDifficultyDuration = 20000; // time interval when difficulty increases (enemy spawned are better)

  extern const Weapon kSniper("Sniper", bullet, 0.0f, 1000,
                      ProjectileBlueprint(10.0f, 120,
                                          30.0f, false, 0.0f));

  extern const Weapon kMachineGun(std::string("Machine Gun"), bullet, 1.0f, 50,
                    ProjectileBlueprint(10.0f, 5, 20.0f,
                                        false, 0.0f));

  extern const Weapon kRifle("Rifle", bullet, 0.1f, 250,
             ProjectileBlueprint(15.0f, 25,
                                 25.0f, false, 0.0f));

  extern const Weapon kLaser("Laser", beam, 0.0f, 300,
             ProjectileBlueprint(5.0f, 10,
                                 0, false, 0.0f));


  extern const Weapon kRocket("Rocket", bullet, 0.01f, 1000,
              ProjectileBlueprint(15.0f, 60,
                                  20.0f, true, 100.0f));
}
