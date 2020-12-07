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

        /**
         * Cinder app to handle user input and connection between engine and
         * screen
         */

        class ShooterApp : public ci::app::App {
        public:

            /**
             * Constructor
             */

            ShooterApp();

            void draw() override;
            void update() override;
            void setup() override;
            void keyDown(ci::app::KeyEvent event) override;
            void keyUp(ci::app::KeyEvent event) override;
            void mouseDown(ci::app::MouseEvent event) override;
            void mouseUp(ci::app::MouseEvent event) override;

         private:
            const int kWindowLength = kScreenLength;
            const int kWindowHeight = kScreenHeight;

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
            ci::audio::VoiceRef explosion_sound_;

            /**
             * Helper function to handle firing visuals and call to engine when
             * user fires
             */

            void HandleFiring();

            /**
             * Helper function to handle explosion visuals
             */

            void HandleExplosions();
        };

    }  // namespace visualizer

}  // namespace idealgas

