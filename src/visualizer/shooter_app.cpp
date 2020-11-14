#include "visualizer/shooter_app.h"
#include "cinder/gl/gl.h"
#include <cinder/app/App.h>

namespace shooter {

    namespace visualizer {

        ShooterApp::ShooterApp()
                : engine_(),
                  moves_(),
                  screen_(glm::ivec2(kMargin, kMargin), kWindowLength, kWindowHeight) {

            ci::app::setWindowSize((int) kWindowLength, (int) kWindowHeight);

        }

        void ShooterApp::draw() {
            ci::gl::clear( Color( 0.0f, 0.0f, 0.0f ) );
            ci::gl::color(Color( "white"));
            ci::gl::drawSolidCircle(engine_.GetPlayerPosition(),10);
            ci::gl::color(Color( "red"));
            for (const Bullet& bullet : engine_.get_bullets_()) {
                ci::gl::drawSolidCircle(bullet.get_position_(), bullet.get_radius_());
            }
            ci::gl::color(Color("blue"));
            for (const Enemy& enemy : engine_.get_enemies_()) {
                ci::gl::drawSolidCircle(enemy.get_position_(), enemy.get_radius_());
            }
        }
        void ShooterApp::update() {
            engine_.update(moves_);
        }

        void ShooterApp::setup(){};

        void ShooterApp::keyDown(ci::app::KeyEvent event) {
            switch (event.getChar()) {
                case ci::app::KeyEvent::KEY_w:
                    moves_.insert(up);
                    break;
                case ci::app::KeyEvent::KEY_s:
                    moves_.insert(down);
                    break;
                case ci::app::KeyEvent::KEY_d:
                    moves_.insert(right);
                    break;
                case ci::app::KeyEvent::KEY_a:
                    moves_.insert(left);
                    break;
            }
        }

        void ShooterApp::keyUp(ci::app::KeyEvent event) {
            switch (event.getChar()) {
                case ci::app::KeyEvent::KEY_w:
                    moves_.erase(up);
                    break;
                case ci::app::KeyEvent::KEY_s:
                    moves_.erase(down);
                    break;
                case ci::app::KeyEvent::KEY_d:
                    moves_.erase(right);
                    break;
                case ci::app::KeyEvent::KEY_a:
                    moves_.erase(left);
                    break;
            }
        }
        void ShooterApp::mouseDown(ci::app::MouseEvent event) {
            glm::vec2 cursor = event.getPos();
            engine_.HandleShoot(cursor);
        }
    }
}

