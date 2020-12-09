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

   /**
    * main draw function that is called every frame
    */

  void draw() override;

  /**
    * update function called every frame to update variables
    */

  void update() override;

  /**
   * setup function called at beginning
   */

  void setup() override;

  /**
   * Method to track keys that are pressed down
   * @param event
   */

  void keyDown(ci::app::KeyEvent event) override;

  /**
   * Method to track keys that are released
   * @param event
   */

  void keyUp(ci::app::KeyEvent event) override;

  /**
   * Method to track mouse buttons that are pressed down
   * @param event
   */
  void mouseDown(ci::app::MouseEvent event) override;

  /**
   * Method to track mouse buttons that are released
   * @param event
   */
  void mouseUp(ci::app::MouseEvent event) override;

 private:

  const int kWindowLength = kScreenLength;
  const int kWindowHeight = kScreenHeight;

  std::set<Direction> moves_;
  Engine engine_;
  Screen screen_;
  bool firing_;
  bool running_;

  int get_new_explosions_counter_;
  std::vector<std::pair<glm::vec2,float>> explosions_;

  int is_beam_;
  glm::vec2 beam_cursor_location_;

  // audio

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

