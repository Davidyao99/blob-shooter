#include "visualizer/screen.h"


namespace shooter {

  using namespace ci;

  namespace visualizer {

    Screen::Screen(glm::ivec2 top_left, int screen_length,
                 int screen_height, const glm::ivec2 engine_dimensions) :
    kTopLeft(top_left), kLength(screen_length), kHeight(screen_height),
    kCenter(screen_length / 2, screen_height / 2),
                kEngineDimensions(engine_dimensions){}

    void Screen::Draw(const Player& player, const std::vector<Enemy> &enemies,
                      const std::vector<Bullet> &bullets) {
      ci::gl::color(ci::Color("black"));
      vec2 bottom_right = kTopLeft + glm::ivec2(kLength, kHeight);
      ci::Rectf pixel_bounding_box(kTopLeft, bottom_right);

      ci::gl::drawSolidRect(pixel_bounding_box);
      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);

      DrawPlayer(player);
      DrawEnemies(enemies, player.get_position_());
      DrawBullets(bullets, player.get_position_());
      DrawBoundaries(player);
    }

  void Screen::DrawPlayer(const Player& player) const {
    float reload_status = player.GetReloadStatus();
    glm::ivec2 player_screen_position(kCenter + kTopLeft);
    glm::vec2 reload_bar_top_left(player_screen_position.x - 20,
                                  player_screen_position.y - player.get_radius_() - 8);
    glm::vec2 reload_bar_bottom_right = reload_bar_top_left + glm::vec2(
        40.0f * reload_status,
        4
    );
    ci::Rectf reload_bar(static_cast<glm::ivec2>(reload_bar_top_left),
                         static_cast<glm::ivec2>(reload_bar_bottom_right));
    ci::gl::color(Color( "white"));
    ci::gl::drawSolidRect(reload_bar);
    ci::gl::color(Color(0.0f,1.0f, 1.0f));
    ci::gl::drawSolidCircle( player_screen_position,
                             player.get_radius_());
  }

  void Screen::ScreenProcessBoundary(glm::ivec2 &screen_position) const {
    if (screen_position.x < 0) {
      screen_position.x = 0;
    }
    if (screen_position.y < 0) {
      screen_position.y = 0;
    }
    if (screen_position.x > kLength) {
      screen_position.x = kLength;
    }
    if (screen_position.y > kHeight) {
      screen_position.y = kHeight;
    }
  }

  void Screen::DrawBoundaries(const Player& player) const {
    const glm::ivec2 &player_position = static_cast<glm::ivec2>(player.get_position_());

    glm::ivec2 top_left_engine = GetScreenPosition(glm::ivec2(0,0),
                                                        player.get_position_());
    ScreenProcessBoundary(top_left_engine);
    glm::ivec2 bottom_right_engine =
        GetScreenPosition(kEngineDimensions,
                                    player.get_position_());
    ScreenProcessBoundary(bottom_right_engine);
    glm::ivec2 top_right_engine =
        GetScreenPosition(glm::ivec2(kEngineDimensions.x,0),
                                             player.get_position_());
    ScreenProcessBoundary(top_right_engine);
    glm::ivec2 bottom_left_engine =
        GetScreenPosition(glm::ivec2(0, kEngineDimensions.y),
                                                       player.get_position_());
    ScreenProcessBoundary(bottom_left_engine);

    ci::gl::color(Color("green"));
    if (top_left_engine.x != 0) {
      ci::Rectf boundary1(top_left_engine + kTopLeft,
                         bottom_left_engine + kTopLeft +
                         glm::ivec2(-5,0));
      ci::gl::drawSolidRect(boundary1);
    }
    if (top_left_engine.y != 0) {
      ci::Rectf boundary1(top_left_engine + kTopLeft,
                          top_right_engine+ kTopLeft +
                          glm::ivec2(0,-5));
      ci::gl::drawSolidRect(boundary1);
    }
    if (bottom_right_engine.x != kLength) {
      ci::Rectf boundary1(bottom_right_engine + kTopLeft,
                          top_right_engine+ kTopLeft +
                          glm::ivec2(5,0));
      ci::gl::drawSolidRect(boundary1);
    }
    if (bottom_right_engine.y != kHeight) {
      ci::Rectf boundary1(bottom_right_engine + kTopLeft,
                          bottom_left_engine+ kTopLeft +
                          glm::ivec2(0,5));
      ci::gl::drawSolidRect(boundary1);
    }



//    const glm::ivec2 &player_position = static_cast<glm::ivec2>(player.get_position_());
//    glm::ivec2 top_left_engine = GetScreenPosition(glm::ivec2(0,0),
//                                                        player.get_position_());
//    glm::ivec2 bottom_right_engine = GetScreenPosition(kEngineDimensions,
//                                                        player.get_position_());
//    bool no_boundaries = true;
//    if (PositionInBound(top_left_engine)) {
//      ci::gl::color(Color("green"));
//      ci::Rectf boundary1(top_left_engine + kTopLeft,
//                         top_left_engine + kTopLeft +
//                         glm::ivec2(kLength - top_left_engine.x,5));
//      ci::gl::drawSolidRect(boundary1);
//      ci::Rectf boundary2 (top_left_engine + kTopLeft,
//                          top_left_engine + kTopLeft
//                              + glm::ivec2(-5,
//                                            kHeight - top_left_engine.y));
//
//      ci::gl::drawSolidRect(boundary2);
//      no_boundaries = false;
//    }
//    if (PositionInBound(bottom_right_engine)) {
//      ci::gl::color(Color("green"));
//
//      ci::Rectf boundary1(bottom_right_engine + kTopLeft,
//                          bottom_right_engine + kTopLeft +
//                                       glm::ivec2(-bottom_right_engine.x,5));
//
//      ci::gl::drawSolidRect(boundary1);
//      ci::Rectf boundary2 (bottom_right_engine + kTopLeft,
//                           bottom_right_engine + kTopLeft +
//                           glm::ivec2(5, -bottom_right_engine.y));
//      ci::gl::drawSolidRect(boundary2);
//      no_boundaries = false;
//    }
//    if (no_boundaries) {
//      if (player_position.x < kCenter.x) {
//        ci::Rectf boundary1(glm::ivec2(player_position.x,0) + kTopLeft,
//                            glm::ivec2(player_position.x, kHeight)+
//                            glm::ivec2(-5,0) + kTopLeft);
//        ci::gl::drawSolidRect(boundary1);
//      }
//    }

  }

  void Screen::DrawEnemies(const std::vector<Enemy> &enemies,
                           const glm::vec2 player)  const {
    for (const Enemy& enemy : enemies) {
      glm::ivec2 screen_position = GetScreenPosition(enemy.get_position_(), player);
      if (PositionInBound(screen_position)) {
        int hit_points = enemy.get_hit_points_();
        float bluishness = static_cast<float>(hit_points) / 10;
        ci::gl::color(Color(1.0f-bluishness,1.0f-bluishness,1.0f));
        ci::gl::drawSolidCircle(
            screen_position + kTopLeft,
            enemy.get_radius_());
      }
    }
  }

  void Screen::DrawBullets(const std::vector<Bullet> &bullets,
                           const glm::vec2 player) const {
    ci::gl::color(Color( "red"));

    for (const Bullet& bullet : bullets) {
      glm::ivec2 screen_position = GetScreenPosition(bullet.get_position_(), player);
      if (PositionInBound(screen_position)) {
        ci::gl::drawSolidCircle(
            screen_position + kTopLeft,
            bullet.get_radius_());
      }
    }
  }

    bool Screen::PositionInBound(const glm::ivec2 position) const{
      return (position.y < kLength && position.y > 0 &&
          position.x > 0 && position.x < kHeight);
    }

    glm::ivec2 Screen::GetScreenPosition(const glm::vec2 target_position,
                                               const glm::vec2 player) const {
      glm::ivec2 relative_position_with_player =
          static_cast<glm::ivec2>(target_position - player);

      return kCenter + relative_position_with_player;
    }

    const glm::ivec2 Screen::GetCenter() const {
      return kCenter + kTopLeft;
    }

    const glm::ivec2 Screen::get_kTopLeft() const {
      return kTopLeft;
    }


  }


}
