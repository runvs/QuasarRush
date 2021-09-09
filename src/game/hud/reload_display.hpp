#ifndef QUASARRUSH_RELOAD_DISPLAY_HPP
#define QUASARRUSH_RELOAD_DISPLAY_HPP

#include "bar.hpp"
#include "observer_interface.hpp"

class ReloadDisplay : public ObserverInterface<float> {
public:
    explicit ReloadDisplay(std::shared_ptr<jt::Bar> bar);
    void notify(float value) override;

private:
    std::shared_ptr<jt::Bar> m_bar;
};

#endif // QUASARRUSH_RELOAD_DISPLAY_HPP
