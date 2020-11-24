#pragma once

#include <cinder/app/App.h>
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "core/engine.h"
#include "cinder/gl/Texture.h"
#include "visualizer/screen.h"

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

        private:
            const int kWindowLength = 1200;
            const int kWindowHeight = 1000;
            const int kMargin = 5;

            const int kScreenLength = 1195;
            const int kScreenHeight = 995;

            std::set<Direction> moves_;
            Engine engine_;
            Screen screen_;
        };

    }  // namespace visualizer

}  // namespace idealgas

