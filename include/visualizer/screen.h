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

           Screen(glm::ivec2 top_left, int screen_length, int screen_height);

           void Draw(const Player& player,
                     const std::vector<Enemy> &enemies,
                     const std::vector<Bullet>& bullets);
            /**
             * Checks if position within bound, if outside, stop drawing item
             * @param position position to be examined
             * @return true if in-bound, false otherwise
             */
            bool PositionInBound(const glm::ivec2 position) const;

            //Todo to be used for camera scrolling

//            const glm::ivec2 Screen::GetCenter() const;
//
//            glm::ivec2 GetEntityScreenPosition(const Entity& entity,
//                                                  const glm::vec2 player) const;
            // getters
            const glm::ivec2 get_kTopLeft() const;




          private:


           const glm::ivec2 kTopLeft;
           const glm::ivec2 kCenter; // always center of screen
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
           void DrawEnemies(const std::vector<Enemy> &enemies) const;

           /**
            * Draws bullets
            * @param bullets
            */
           void DrawBullets(const std::vector<Bullet> &bullets) const;

        };

    }

}