#ifndef QUASARRUSH_STATE_MENU_BASE_HPP
#define QUASARRUSH_STATE_MENU_BASE_HPP

#include "game_state.hpp"
#include "render_target.hpp"
#include <functional>
#include <memory>

namespace jt {
class Shape;
class Sprite;
class Text;
class GameState;
} // namespace jt

class StateMenuBase {
private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Sprite> m_quasarImage;
    std::shared_ptr<jt::Text> m_text_Title;

    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;

public:
    void create(std::shared_ptr<jt::renderTarget> rt, jt::GameState& state);
    void update(float elapsed);
    void draw(std::shared_ptr<jt::renderTarget> rt);

    void drawOverlay(std::shared_ptr<jt::renderTarget> rt);

    void startFadeOut(std::function<void(void)> callback, jt::GameState& state);
};

#endif // QUASARRUSH_STATE_MENU_BASE_HPP
