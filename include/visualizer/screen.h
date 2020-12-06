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

           /**
            * Main drawing function that calls other helper drawing functions
            * @param player const reference of player object
            * @param enemies vector of enemies to be drawn
            * @param bullets vector of bullets to be drawn
            * @param explosions vector of pairs containing position of explosion
            * and radius for drawing
            * @param score score to be displayed
            */

           void Draw(const Player& player,
                     const std::vector<Enemy> &enemies,
                     const std::vector<Bullet>& bullets,
                     const std::vector<std::pair<glm::vec2,float>> &explosions,
                     int score);

           /**
            * Draws a beam
            * @param cursor_position
            * @param radius 0.5 the width of beam
            */

           void DrawBeam(glm::vec2 cursor_position);

            /**
             * Checks if position within bound, if outside, stop drawing item
             * @param position position to be examined
             * @return true if in-bound, false otherwise
             */

            bool PositionInBound(const glm::ivec2 position, int radius) const;

            /**
             * converts corner positions from actual game to positions relative to player
             * to be drawn on the screen
             * @param target_position actual position in game
             * @param player actual position of player
             * @return returns relative position from player
             */
            glm::ivec2 GetScreenPosition(const glm::vec2 target_position,
                                                  const glm::vec2 player) const;


            void DrawLoseScene() const;

            // getters
            const glm::ivec2 get_kTopLeft() const;
            const glm::ivec2 Screen::get_kCenter_() const;


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

           /**
            * Draws explosions
            * @param explosions
            * @param player_position
            */

          void DrawExplosions(const std::vector<std::pair<glm::vec2,float>> &explosions,
                               const glm::vec2 player_position);


          /**
           * Ensures that the screen_position is in bound
           * @param screen_position
           */
          void ScreenProcessBoundary(glm::ivec2 &screen_position) const;

          /**
           * Draws the main criss-cross grids in the background
           * @param player
           */
          void DrawGrid(const glm::ivec2 player) const;


        };

    }

}