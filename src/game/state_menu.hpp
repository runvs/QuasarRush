#ifndef GAME_STATE_MENU_HPP_INCLUDEGUARD
#define GAME_STATE_MENU_HPP_INCLUDEGUARD

#include "game_state.hpp"
#include "player_config.hpp"
#include "state_menu_base.hpp"
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
    std::shared_ptr<StateMenuBase> m_menuBase;

    std::vector<std::shared_ptr<jt::Button>> m_buttons;

    std::string m_levelFilename{""};
    
    std::shared_ptr<jt::Button> m_buttonIncreaseSensors;
    std::shared_ptr<jt::Button> m_buttonIncreaseEngine;

    std::shared_ptr<jt::Button> m_buttonSwitchToMG;
    std::shared_ptr<jt::Button> m_buttonSwitchToMissile;

    std::shared_ptr<jt::Text> m_textSensor;
    std::shared_ptr<jt::Text> m_textEngine;

    std::shared_ptr<jt::Button> m_buttonCredits;

    bool m_started { false };

    void doInternalCreate() override;


    void createTweenTransition();

    void doInternalUpdate(float const elapsed) override;
    void updateDrawables(const float& elapsed);
    void startTransitionToStateGame(std::string const& levelFilename);

    void doInternalDraw() const override;
    void createLevelButtons();
    void createShipUpgradeButtons();
    void updateShipUpgradeButtons(float const elapsed);
};

#endif
