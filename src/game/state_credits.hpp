#ifndef QUASARRUSH_STATE_CREDITS_HPP
#define QUASARRUSH_STATE_CREDITS_HPP

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

public:
    void doInternalCreate() override;
    void doInternalUpdate(float elapsed) override;
    void doInternalDraw() const override;
};

#endif // QUASARRUSH_STATE_CREDITS_HPP
