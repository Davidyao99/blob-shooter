#pragma once

#include <cinder/app/App.h>
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "core/engine.h"
#include "cinder/gl/Texture.h"
#include "visualizer/screen.h"
#include <cinder/audio/audio.h>

namespace shooter {
    using namespace ci;

    namespace visualizer {

        class ShooterApp : public ci::app::App {
        public:
            ShooterApp();

            void draw() override;
            void update() override;
            void setup() override;
            void keyDown(ci::app::KeyEvent event) override;
            void keyUp(ci::app::KeyEvent event) override;
            void mouseDown(ci::app::MouseEvent event) override;
            void mouseUp(ci::app::MouseEvent event) override;

         private:
            const int kWindowLength = 1200;
            const int kWindowHeight = 1000;

            const int kScreenLength = 1200;
            const int kScreenHeight = 1000;

            std::set<Direction> moves_;
            Engine engine_;
            Screen screen_;
            bool firing_;
            bool running_;

            bool get_new_explosions_;
            std::vector<std::pair<glm::vec2,float>> explosions_;

            int is_beam_;
            glm::vec2 beam_cursor_location_;

            ci::audio::VoiceRef bullet_sound_;
            ci::audio::VoiceRef laser_sound_;
        };

    }  // namespace visualizer

}  // namespace idealgas

