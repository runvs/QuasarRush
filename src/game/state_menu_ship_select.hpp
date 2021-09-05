#ifndef QUASARRUSH_STATE_MENU_SHIP_SELECT_HPP
#define QUASARRUSH_STATE_MENU_SHIP_SELECT_HPP

#include "player_config.hpp"
#include "state_menu_base.hpp"
#include "game_state.hpp"

namespace jt {

class Text;
class Shape;
class Animation;
class Sprite;
class Button;

} // namespace jt

class StateMenuShipSelect : public jt::GameState{
public:
    StateMenuShipSelect();
    void setPlayerConfig(PlayerConfig const& pc);
private:
    std::shared_ptr<StateMenuBase> m_menuBase;
    bool m_started { false };

    std::shared_ptr<jt::Button> m_buttonBack;
    std::shared_ptr<jt::Button> m_buttonFly;

    std::shared_ptr<jt::Button> m_buttonIncreaseSensors;
    std::shared_ptr<jt::Button> m_buttonIncreaseEngine;

    std::shared_ptr<jt::Button> m_buttonSwitchToMG;
    std::shared_ptr<jt::Button> m_buttonSwitchToMissile;

    std::shared_ptr<jt::Text> m_textSensor;
    std::shared_ptr<jt::Text> m_textEngine;

    std::shared_ptr<jt::Text> m_textPointsAvailable;


    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void startTransitionToStateGame();
    void createTweenTransition();

    void createShipUpgradeButtons();
    void updateShipUpgradeButtons(float const elapsed);
    void createButtonBack();
    void createButtonFly();
    void updateTexts(float const elapsed);
};

#endif // QUASARRUSH_STATE_MENU_SHIP_SELECT_HPP
