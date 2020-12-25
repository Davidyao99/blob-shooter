#include "visualizer/screen.h"


namespace shooter {

  using namespace ci;

  namespace visualizer {

    Screen::Screen(glm::ivec2 top_left, int screen_length, int screen_height) :
    kTopLeft(top_left), kLength(screen_length), kHeight(screen_height),
    kCenter(screen_length / 2, screen_height / 2){}

    void Screen::Draw(const Player& player, const std::vector<Enemy> &enemies,
                      const std::vector<Bullet> &bullets) {
      ci::gl::color(ci::Color("black"));
      vec2 bottom_right = kTopLeft + glm::ivec2(kLength, kHeight);
      ci::Rectf pixel_bounding_box(kTopLeft, bottom_right);

      ci::gl::drawSolidRect(pixel_bounding_box);
      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);

      DrawPlayer(player);
      DrawEnemies(enemies);
      DrawBullets(bullets);
    }

  void Screen::DrawPlayer(const Player& player) const {
    float reload_status = player.GetReloadStatus();
    glm::ivec2 player_position(player.get_position_());
    glm::vec2 reload_bar_top_left(player_position.x - 20,
                                  player_position.y - player.get_radius_() - 8);
    glm::vec2 reload_bar_bottom_right = reload_bar_top_left + glm::vec2(
        40.0f * reload_status,
        4
    );
    ci::Rectf reload_bar(static_cast<glm::ivec2>(reload_bar_top_left) + kTopLeft,
                         static_cast<glm::ivec2>(reload_bar_bottom_right) + kTopLeft);
    ci::gl::color(Color( "white"));
    ci::gl::drawSolidRect(reload_bar);
    ci::gl::color(Color(0.0f,1.0f, 1.0f));
    ci::gl::drawSolidCircle( player_position + kTopLeft,
                             player.get_radius_());
  }

  void Screen::DrawEnemies(const std::vector<Enemy> &enemies)  const {
    for (const Enemy& enemy : enemies) {
//        glm::ivec2 screen_position = GetEntityScreenPosition(enemy, player);
      glm::ivec2 enemy_position = enemy.get_position_();
      if (PositionInBound(enemy_position)) {
        int hit_points = enemy.get_hit_points_();
        float bluishness = static_cast<float>(hit_points) / 10;
        ci::gl::color(Color(1.0f-bluishness,1.0f-bluishness,1.0f));
        ci::gl::drawSolidCircle(
            enemy_position + kTopLeft,
            enemy.get_radius_());
      }
    }
  }

  void Screen::DrawBullets(const std::vector<Bullet> &bullets) const {
    ci::gl::color(Color( "red"));

    for (const Bullet& bullet : bullets) {
//        glm::ivec2 screen_position = GetEntityScreenPosition(bullet, player);
      glm::ivec2 bullet_position = bullet.get_position_();
      if (PositionInBound(bullet_position)) {
        ci::gl::drawSolidCircle(
            bullet_position + kTopLeft,
            bullet.get_radius_());
      }
    }
  }

    bool Screen::PositionInBound(const glm::ivec2 position) const{
      return (position.y < kLength && position.y > 0 && position.x > 0 &&
              position.x < kHeight);
    }

    //Todo to be used for camera scrolling
//    glm::ivec2 Screen::GetEntityScreenPosition(const Entity& entity,
//                                               const glm::vec2 player) const {
//      glm::vec2 position = entity.get_position_();
//
//      glm::ivec2 relative_position_with_player =
//          static_cast<glm::ivec2>(position - player);
//
//      return kCenter + relative_position_with_player;
//    }

//    const glm::ivec2 Screen::GetCenter() const {
//      return kCenter + kTopLeft;
//    }

    const glm::ivec2 Screen::get_kTopLeft() const {
      return kTopLeft;
    }


  }


}
