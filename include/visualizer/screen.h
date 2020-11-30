#pragma once

#include <cinder/app/App.h>
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "core/engine.h"
#include "cinder/gl/Texture.h"

namespace shooter {

    using namespace ci;

    namespace visualizer {

        class Screen {

          public:

           Screen(int screen_length,
                  int screen_height, const glm::ivec2 board_dimensions);

           void Draw(const Player& player,
                     const std::vector<Enemy> &enemies,
                     const std::vector<Bullet>& bullets,
                     const std::vector<std::pair<glm::vec2,float>> &explosions,
                     int score);

           void DrawBeam(glm::vec2 cursor_position, float radius);
            /**
             * Checks if position within bound, if outside, stop drawing item
             * @param position position to be examined
             * @return true if in-bound, false otherwise
             */
            bool PositionInBound(const glm::ivec2 position) const;

            const glm::ivec2 GetCenter() const;

            glm::ivec2 GetScreenPosition(const glm::vec2 target_position,
                                                  const glm::vec2 player) const;

            // getters
            const glm::ivec2 get_kTopLeft() const;


          private:


           const glm::ivec2 kTopLeft;
           const glm::ivec2 kCenter; // always center of screen
           const glm::ivec2 kEngineDimensions;
           const int kLength;
           const int kHeight;

           /**
            * Draw player and reload bar
            * @param player
            */
           void DrawPlayer(const Player& player) const;

           /**
            * Draw enemies
            * @param enemies
            */
           void DrawEnemies(const std::vector<Enemy> &enemies,
                            const glm::vec2 player) const;

           /**
            * Draws bullets
            * @param bullets
            */
           void DrawBullets(const std::vector<Bullet> &bullets,
                            const glm::vec2 player) const;

           /**
            * Draw boundary lines
            * @param player
            */
           void DrawBoundaries(const Player& player) const;

          void DrawExplosions(const std::vector<std::pair<glm::vec2,float>> &explosions,
                               const glm::vec2 player_position);

          void ScreenProcessBoundary(glm::ivec2 &screen_position) const;

           void DrawGrid(const glm::ivec2 player) const;


        };

    }

}