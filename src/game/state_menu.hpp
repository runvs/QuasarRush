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

    void setPlayerConfig(PlayerConfig const& pc);

private:
    std::shared_ptr<StateMenuBase> m_menuBase;

    bool m_started { false };

    std::shared_ptr<jt::Button> m_buttonCredits;
    std::shared_ptr<jt::Button> m_buttonSelectLevel;

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void startTransitionToStateSelectLevel();
    void createTweenTransition();

    void createShipUpgradeButtons();
    void updateShipUpgradeButtons(float const elapsed);
    void createButtonCredits();
    void createButtonSelectLevel();
};

#endif
