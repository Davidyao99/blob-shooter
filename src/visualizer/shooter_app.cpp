#include "visualizer/shooter_app.h"
#include "cinder/gl/gl.h"
#include <cinder/app/App.h>

namespace shooter {

    namespace visualizer {

        ShooterApp::ShooterApp()
                : engine_(static_cast<float>(kScreenLength*2),
                          static_cast<float>(kScreenHeight*2)),
                  moves_(),
                  screen_(kScreenLength,kScreenHeight,
                  engine_.get_board_dimensions_()),
                  firing_(false),
                  is_beam_(false){

            ci::app::setWindowSize( kWindowLength, kWindowHeight);
        }

        void ShooterApp::draw() {

          ci::Color8u background_color(255, 246, 148);  // light yellow
          ci::gl::clear(background_color);

          screen_.Draw(engine_.get_player_(),
                         engine_.get_enemies_(),
                         engine_.get_bullets_(),
                       engine_.get_explosives_(),
                       engine_.get_score_());

          engine_.ClearExplosives();

          if (is_beam_) {
            screen_.DrawBeam(getMousePos()-getWindowPos(),
                engine_.get_player_().GetCurrentWeapon().get_projectile_blueprint_().radius_);
            is_beam_ = false;
          }
        }

        void ShooterApp::update() {
          engine_.update(moves_);
          if (firing_) {
            // gets cursor relative pos to player
            glm::vec2 cursor_relative_pos =
                getMousePos() - getWindowPos() - screen_.GetCenter();
            if (engine_.Reloaded()) {
              ProjectileType type = engine_.HandleShoot(cursor_relative_pos);
              is_beam_ = type == beam;
            }
          }
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
                case ci::app::KeyEvent::KEY_q:
                  engine_.ChangeWeapon(true);
                  break;
                case ci::app::KeyEvent::KEY_e:
                  engine_.ChangeWeapon(false);
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
          firing_ = true;
        }

        void ShooterApp::mouseUp(ci::app::MouseEvent event) {
          firing_ = false;
        }
    }
}