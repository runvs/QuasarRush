#ifndef QUASARRUSH_STATE_MENU_LEVEL_SELECT_HPP
#define QUASARRUSH_STATE_MENU_LEVEL_SELECT_HPP

#include "state_menu_base.hpp"
#include "game_state.hpp"
#include <memory>

namespace jt {
class Button;

} // namespace jt

class StateMenuLevelSelect  : public jt::GameState {
public:
    StateMenuLevelSelect();
    void setPlayerConfig(PlayerConfig const& pc);

private:
    std::shared_ptr<StateMenuBase> m_menuBase;

    std::vector<std::shared_ptr<jt::Button>> m_buttons;
    std::shared_ptr<jt::Button> m_buttonBack;

    bool m_started { false };

    void doInternalCreate() override;
    void createLevelButtons();

    void startTransitionToStateGame(std::string const& levelFilename);
    void createTweenTransition();

    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void createButtonBack();
};

#endif // QUASARRUSH_STATE_MENU_LEVEL_SELECT_HPP
