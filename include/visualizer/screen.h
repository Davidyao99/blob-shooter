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

            bool PositionInBound(const glm::ivec2 position) const;

            const glm::ivec2 Screen::GetCenter() const;

            glm::ivec2 GetEntityScreenPosition(const Entity& entity,
                                                  const glm::vec2 player) const;

            const glm::ivec2 get_kTopLeft() const;


          private:

           const glm::ivec2 kTopLeft;
           const glm::ivec2 kCenter; // always center of screen
           const int kLength;
           const int kHeight;

        };

    }

}