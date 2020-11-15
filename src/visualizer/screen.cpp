#include "visualizer/screen.h"


namespace shooter {

  using namespace ci;

  namespace visualizer {

    Screen::Screen(glm::ivec2 top_left, int screen_length, int screen_height) :
    kTopLeft(top_left), kLength(screen_length), kHeight(screen_height),
    kPlayerLocation(screen_length / 2, screen_height / 2){}

    void Screen::Draw(glm::vec2 player, std::vector<Enemy> enemies,
                      std::vector<Bullet> bullets) {
      ci::gl::color(ci::Color("black"));
      vec2 bottom_right = kTopLeft + glm::ivec2(kLength, kHeight);
      ci::Rectf pixel_bounding_box(kTopLeft, bottom_right);

      ci::gl::drawSolidRect(pixel_bounding_box);
      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);

      ci::gl::color(Color( "white"));
      ci::gl::drawSolidCircle(kPlayerLocation + kTopLeft,
                              10);
      ci::gl::color(Color( "red"));
      for (const Bullet& bullet : bullets) {
        glm::ivec2 screen_position = GetEntityScreenPosition(bullet, player);
        if (PositionInBound(screen_position)) {
          ci::gl::drawSolidCircle(
              screen_position + kTopLeft,
              bullet.get_radius_());
        }
      }
      ci::gl::color(Color("blue"));
      for (const Enemy& enemy : enemies) {
        glm::ivec2 screen_position = GetEntityScreenPosition(enemy, player);
        if (PositionInBound(screen_position)) {
          ci::gl::drawSolidCircle(
              screen_position + kTopLeft,
              enemy.get_radius_());
        }
      }

    }

    bool Screen::PositionInBound(const glm::ivec2 position) const{
      return (position.y < kLength && position.y > 0 && position.x > 0 &&
              position.x < kHeight);
    }

    glm::ivec2 Screen::GetEntityScreenPosition(const Entity& entity,
                                               const glm::vec2 player) const {
      glm::vec2 position = entity.get_position_();

      glm::ivec2 relative_position_with_player =
          static_cast<glm::ivec2>(position - player);

      return kPlayerLocation + relative_position_with_player;
    }


  }


}
