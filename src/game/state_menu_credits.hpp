#ifndef QUASARRUSH_STATE_MENU_CREDITS_HPP
#define QUASARRUSH_STATE_MENU_CREDITS_HPP

#include "game_state.hpp"
#include "state_menu_base.hpp"
#include <memory>

namespace jt {
class Text;
} // namespace jt

class StateCredits : public jt::GameState {
private:
    std::shared_ptr<StateMenuBase> m_menuBase;
    std::shared_ptr<jt::Text> m_text_Credits;
    bool m_started { false };

public:
    StateCredits();
    void doInternalCreate() override;
    void doInternalUpdate(float elapsed) override;
    void doInternalDraw() const override;
    void setPlayerConfig(PlayerConfig const& pc);
    void startTransitionToStateMenu();
    void createTweenTransition();
};

#endif // QUASARRUSH_STATE_MENU_CREDITS_HPP
