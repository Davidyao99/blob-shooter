#include "visualizer/screen.h"


namespace shooter {

  using namespace ci;

  namespace visualizer {

    Screen::Screen(glm::ivec2 top_left, int screen_length, int screen_height) :
    kTopLeft(top_left), kLength(screen_length), kHeight(screen_height),
    kCenter(screen_length / 2, screen_height / 2){}

    void Screen::Draw(glm::vec2 player, std::vector<Enemy> enemies,
                      std::vector<Bullet> bullets) {
      ci::gl::color(ci::Color("black"));
      vec2 bottom_right = kTopLeft + glm::ivec2(kLength, kHeight);
      ci::Rectf pixel_bounding_box(kTopLeft, bottom_right);

      ci::gl::drawSolidRect(pixel_bounding_box);
      ci::gl::color(ci::Color("black"));
      ci::gl::drawStrokedRect(pixel_bounding_box);

      ci::gl::color(Color( "white"));
      ci::gl::drawSolidCircle(static_cast<glm::ivec2>(player) + kTopLeft,
                              10);
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
      for (const Enemy& enemy : enemies) {
//        glm::ivec2 screen_position = GetEntityScreenPosition(enemy, player);
        glm::ivec2 enemy_position = enemy.get_position_();
        if (PositionInBound(enemy_position)) {
          int hit_points = enemy.get_hit_points_();
          std::cout<<hit_points<<std::endl;
          float bluishness = static_cast<float>(hit_points) / 10;
          ci::gl::color(Color(1.0f-bluishness,1.0f-bluishness,1.0f));
          ci::gl::drawSolidCircle(
              enemy_position + kTopLeft,
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
