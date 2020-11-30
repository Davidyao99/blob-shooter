#include "visualizer/screen.h"


namespace shooter {

  using namespace ci;

  namespace visualizer {

    Screen::Screen(int screen_length,
                 int screen_height, const glm::ivec2 engine_dimensions) :
    kLength(screen_length), kHeight(screen_height),
    kCenter(screen_length / 2, screen_height / 2),
                kEngineDimensions(engine_dimensions){}

    void Screen::Draw(const Player& player, const std::vector<Enemy> &enemies,
                      const std::vector<Bullet> &bullets,
                      const std::vector<std::pair<glm::vec2,float>> &explosions,
                      int score) {
      ci::gl::color(ci::Color(0.1f,0.1f,0.1f));
      vec2 bottom_right = glm::ivec2(kLength, kHeight);
      ci::Rectf pixel_bounding_box(glm::vec2(0,0), bottom_right);

      ci::gl::drawSolidRect(pixel_bounding_box);
      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);

      DrawGrid(player.get_position_());
      DrawPlayer(player);
      DrawEnemies(enemies, player.get_position_());
      DrawBullets(bullets, player.get_position_());
      DrawExplosions(explosions, player.get_position_());
      DrawBoundaries(player);

      vec2 score_display = glm::ivec2(kLength/2, 30);
      std::string score_str = std::to_string(score);
      ci::gl::drawStringCentered(score_str, score_display,
                                 ci::Color(0.2f,1.0f,0.2f),
                                 ci::Font("monospace", 64));
    }

  void Screen::DrawExplosions(const std::vector<std::pair<glm::vec2,float>> &explosions,
                                const glm::vec2 player_position) {
    for (auto explosion : explosions) {
      ci::gl::color(Color("Yellow"));
      ci::gl::drawSolidCircle(GetScreenPosition(explosion.first,player_position)+kTopLeft,
                               explosion.second);
    }
  }

  void Screen::DrawPlayer(const Player& player) const {
    float reload_status = player.GetWeaponReloadStatus();
    glm::ivec2 player_screen_position(kCenter);
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
    glm::vec2 weapon_string_location(player_screen_position.x,
                                     player_screen_position.y -
                                         player.get_radius_() - 20);
    std::string weapon_name = player.GetCurrentWeapon().get_name_();
    ci::gl::drawStringCentered(weapon_name, weapon_string_location,
                               ci::Color("white"),
                               ci::Font("monospace", 16));

    ci::gl::color(Color(0.0f,1.0f, 1.0f));
    ci::gl::drawSolidCircle( player_screen_position,
                             player.get_radius_());

    std::string player_health = std::to_string(player.get_health_());
    ci::gl::drawStringCentered(player_health, player_screen_position +
                                                  glm::ivec2(0, -5),
                               ci::Color("green"),
                               ci::Font("monospace", 20));
  }

  void Screen::DrawBeam(glm::vec2 cursor_position, float radius) {
    ci::gl::lineWidth(radius*2.0f);
    ci::gl::color(Color( "red"));
    glm::ivec2 cursor_relative_vec = static_cast<glm::ivec2>(cursor_position) - kCenter;
    ci::gl::drawLine(kCenter,
                     static_cast<glm::ivec2>(cursor_position)+ cursor_relative_vec*100);
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
      ci::Rectf boundary1(top_left_engine,
                         bottom_left_engine +
                         glm::ivec2(-5,0));
      ci::gl::drawSolidRect(boundary1);
    }
    if (top_left_engine.y != 0) {
      ci::Rectf boundary1(top_left_engine,
                          top_right_engine +
                          glm::ivec2(0,-5));
      ci::gl::drawSolidRect(boundary1);
    }
    if (bottom_right_engine.x != kLength) {
      ci::Rectf boundary1(bottom_right_engine,
                          top_right_engine +
                          glm::ivec2(5,0));
      ci::gl::drawSolidRect(boundary1);
    }
    if (bottom_right_engine.y != kHeight) {
      ci::Rectf boundary1(bottom_right_engine,
                          bottom_left_engine +
                          glm::ivec2(0,5));
      ci::gl::drawSolidRect(boundary1);
    }
  }

  void Screen::DrawGrid(const glm::ivec2 player) const {
    ci::gl::color(Color("black"));
    glm::ivec2 screen_in_engine_top_left = player - kCenter; // coords of screen top left in engine
    int x_division = kEngineDimensions.x / 20;
    int y_division = kEngineDimensions.y / 20;

    // prevent negatives when player at boundary
    screen_in_engine_top_left += glm::vec2 (x_division*5, y_division*5);

    int x = x_division - (screen_in_engine_top_left.x % x_division); // x offset
    int y = y_division - (screen_in_engine_top_left.y % y_division); // y offset
    while (x <= kLength) {
      ci::Rectf boundary(glm::ivec2(x,0),
                          glm::ivec2(x+2,kHeight));
      ci::gl::drawSolidRect(boundary);
      x += x_division;
    }
    while (y <= kHeight) {
      ci::Rectf boundary(glm::ivec2(0,y),
                          glm::ivec2(kLength,y+2));
      ci::gl::drawSolidRect(boundary);
      y += y_division;
    }
  }

  void Screen::DrawEnemies(const std::vector<Enemy> &enemies,
                           const glm::vec2 player)  const {
    for (const Enemy& enemy : enemies) {
      glm::ivec2 screen_position = GetScreenPosition(enemy.get_position_(), player);
      if (PositionInBound(screen_position)) {
        int hit_points = enemy.get_health_();
        float bluishness = static_cast<float>(hit_points) / 10;
        ci::gl::color(Color(1.0f-bluishness,1.0f-bluishness,1.0f));
        ci::gl::drawSolidCircle(
            screen_position,
            enemy.get_radius_());
        ci::gl::drawStringCentered(std::to_string(enemy.get_health_()), screen_position,
                                   ci::Color("white"),
                                   ci::Font("monospace", 20));
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
            screen_position,
            bullet.get_radius_());
      }
    }
  }

    bool Screen::PositionInBound(const glm::ivec2 position) const{
      return (position.y < kHeight && position.y > 0 &&
          position.x > 0 && position.x < kLength);
    }

    glm::ivec2 Screen::GetScreenPosition(const glm::vec2 target_position,
                                               const glm::vec2 player) const {
      glm::ivec2 relative_position_with_player =
          static_cast<glm::ivec2>(target_position - player);

      return kCenter + relative_position_with_player;
    }

    const glm::ivec2 Screen::GetCenter() const {
      return kCenter;
    }

    const glm::ivec2 Screen::get_kTopLeft() const {
      return kTopLeft;
    }
  }
}
