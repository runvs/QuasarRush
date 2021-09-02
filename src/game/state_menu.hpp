#ifndef GAME_STATE_MENU_HPP_INCLUDEGUARD
#define GAME_STATE_MENU_HPP_INCLUDEGUARD

#include "game_state.hpp"
#include "player_config.hpp"
#include <string>

namespace jt {

class Text;
class Shape;
class Animation;
class Sprite;
class Button;

} // namespace jt

class StateMenu : public jt::GameState {
public:
    StateMenu();

    void setScore(int /*s*/) {};
    void setPlayerConfig(PlayerConfig const& pc);

private:
    std::shared_ptr<jt::Shape> m_background;

    std::shared_ptr<jt::Text> m_text_Title;
    std::shared_ptr<jt::Text> m_text_Credits;

    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;

    std::vector<std::shared_ptr<jt::Button>> m_buttons;

    std::string m_levelFilename{""};
    PlayerConfig m_playerConfig;

    bool m_started { false };

    void doInternalCreate() override;

    void createVignette();
    void createShapes();

    void createMenuText();
    void createTextCredits();
    void createTextTitle();

    void createTweens();
    void createTweenTitleAlpha();
    void createTweenOverlayAlpha();
    void createTweenCreditsPosition();

    void createTweenTransition();

    void doInternalUpdate(float const elapsed) override;
    void updateDrawables(const float& elapsed);
    void startTransitionToStateGame(std::string const& levelFilename);

    void doInternalDraw() const override;
    void createLevelButtons();
};

#endif
