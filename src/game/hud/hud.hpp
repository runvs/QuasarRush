#ifndef GAME_HUD_HPP_INCLUDEGUARD
#define GAME_HUD_HPP_INCLUDEGUARD

#include "game_object.hpp"
#include "observer_interface.hpp"
#include "sprite.hpp"
#include "text.hpp"
#include "bar.hpp"
#include <tuple>

class TimerDisplay;
class ReloadDisplay;
class HealthDisplay;

class Hud : public jt::GameObject {
public:
    std::shared_ptr<ObserverInterface<float>> getObserverTimer() const;
    std::shared_ptr<ObserverInterface<float>> getObserverReload() const;
    std::shared_ptr<ObserverInterface<std::tuple<float, jt::Vector2>>> getObserverHealth() const;
private:
    std::shared_ptr<TimerDisplay> m_timeDisplay;
    std::shared_ptr<ReloadDisplay> m_reloadDisplay;
    std::shared_ptr<HealthDisplay> m_healthDisplay;

    jt::Text::Sptr m_timeText;
    std::shared_ptr<jt::Bar> m_reloadBar;
    std::shared_ptr<jt::Bar> m_healthBar;
    std::shared_ptr<jt::Sprite> m_healthOverlay;

    void doCreate() override;

    void doUpdate(float const elapsed) override;


    void doDraw() const override;
};

#endif
