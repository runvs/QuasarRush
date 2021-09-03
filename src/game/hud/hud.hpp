#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "game_object.hpp"
#include "observer_interface.hpp"
#include "sprite.hpp"
#include "text.hpp"

class TimerDisplay;

class Hud : public jt::GameObject {
public:
    std::shared_ptr<ObserverInterface<float>> getObserverTimer() const;

private:
    std::shared_ptr<TimerDisplay> m_timeDisplay;

    jt::Text::Sptr m_timeText;

    void doCreate() override;

    void doUpdate(float const elapsed) override;

    void doDraw() const override;
};

#endif
