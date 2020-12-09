#include "visualizer/shooter_app.h"
#include "cinder/gl/gl.h"
#include <cinder/app/App.h>


namespace shooter {

    namespace visualizer {

        ShooterApp::ShooterApp()
                : engine_(static_cast<float>(kWindowLength*kMapSize),
                          static_cast<float>(kWindowHeight*kMapSize)),
                  moves_(),
                  screen_(kWindowLength,kWindowHeight,
                  engine_.get_board_dimensions_()),
                  firing_(false),
                  is_beam_(0),
                  running_(true),
                  get_new_explosions_counter_(5),
                  explosions_(){

            ci::app::setWindowSize( kWindowLength, kWindowHeight);
        }

        void ShooterApp::draw() {

          ci::Color8u background_color(0, 0, 0);
          ci::gl::clear(background_color);

          screen_.Draw(engine_.get_player_(),
                         engine_.get_enemies_(),
                         engine_.get_bullets_(),
                             explosions_,
                       engine_.get_score_());

          if (!running_) { // check if game is running
            screen_.DrawLoseScene();
          }

          if (is_beam_ != 0) { // check if beam is fired
            screen_.DrawBeam(beam_cursor_location_);
            is_beam_ --;
          }
        }

        void ShooterApp::update() {

          if (engine_.PlayerIsDead()) {
            running_ = false;
            return;
          }

          running_ = true;

          HandleFiring();
          HandleExplosions();

          engine_.update(moves_);

        }

        void ShooterApp::HandleFiring() {
          if (!engine_.Reloaded() || !firing_) {
            return;
          }

          // gets cursor relative pos to player
          glm::vec2 cursor_relative_to_player_pos =
              getMousePos() - getWindowPos() - screen_.get_kCenter_();

          ProjectileType type =
              engine_.HandleShoot(cursor_relative_to_player_pos);
          if (type == kBeam) {
            is_beam_ = 3; // draws beam for 3 frames
            beam_cursor_location_ = getMousePos()-getWindowPos();
          }
          if (!is_beam_) {
            bullet_sound_->stop();
            bullet_sound_->start();
          } else {
            laser_sound_->stop();
            laser_sound_->start();
          }

        }

        void ShooterApp::HandleExplosions() {

          get_new_explosions_counter_--; // only get new explosions every 5 frame

          if (get_new_explosions_counter_ == 0) {
            explosions_ = engine_.get_explosions_();
            get_new_explosions_counter_ = 3;
            return;
          }

          if (get_new_explosions_counter_ !=2) {
            return;
          }

          for (auto explosion : explosions_) { // only play sound for first frame
            explosion_sound_->stop();
            explosion_sound_->start();
          }
        }

        void ShooterApp::setup(){

          try {
            ci::audio::SourceFileRef bullet_source_file = audio::load(
                loadAsset("audio/bullet_sound.wav"));
            ci::audio::SourceFileRef laser_source_file = audio::load(
                loadAsset("audio/laser_sound.wav"));
            ci::audio::SourceFileRef explosion_source_file = audio::load(
                loadAsset("audio/explosion_sound.wav"));

            bullet_sound_ = audio::Voice::create(bullet_source_file);
            laser_sound_ = audio::Voice::create(laser_source_file);
            explosion_sound_ = audio::Voice::create(explosion_source_file);

            bullet_sound_->setVolume(1.0f);
            explosion_sound_->setVolume(1.0f);
            laser_sound_->setVolume(1.0f);

          } catch (Exception e) {
            std::cout<<e.what()<<std::endl;
          }
        };

        void ShooterApp::keyDown(ci::app::KeyEvent event) {
            switch (event.getChar()) {
                case ci::app::KeyEvent::KEY_w:
                    moves_.insert(kUp);
                    break;
                case ci::app::KeyEvent::KEY_s:
                    moves_.insert(kDown);
                    break;
                case ci::app::KeyEvent::KEY_d:
                    moves_.insert(kRight);
                    break;
                case ci::app::KeyEvent::KEY_a:
                    moves_.insert(kLeft);
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
                    moves_.erase(kUp);
                    break;
                case ci::app::KeyEvent::KEY_s:
                    moves_.erase(kDown);
                    break;
                case ci::app::KeyEvent::KEY_d:
                    moves_.erase(kRight);
                    break;
                case ci::app::KeyEvent::KEY_a:
                    moves_.erase(kLeft);
                    break;
            }
        }
        void ShooterApp::mouseDown(ci::app::MouseEvent event) {
          firing_ = true;
          if (!running_){
            engine_.Restart();
          }
        }

        void ShooterApp::mouseUp(ci::app::MouseEvent event) {
          firing_ = false;
        }
    }
}