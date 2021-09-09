#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "game_object.hpp"
#include "observer_interface.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "bar.hpp"

class TimerDisplay;
class ReloadDisplay;

class Hud : public jt::GameObject {
public:
    std::shared_ptr<ObserverInterface<float>> getObserverTimer() const;
    std::shared_ptr<ObserverInterface<float>> getObserverReload() const;
private:
    std::shared_ptr<TimerDisplay> m_timeDisplay;
    std::shared_ptr<ReloadDisplay> m_reloadDisplay;

    jt::Text::Sptr m_timeText;
    std::shared_ptr<jt::Bar> m_reloadBar;

    void doCreate() override;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;
};

#endif
