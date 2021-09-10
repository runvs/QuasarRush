#ifndef QUASARRUSH_HEALTH_DISPLAY_HPP
#define QUASARRUSH_HEALTH_DISPLAY_HPP

#include "bar.hpp"
#include "observer_interface.hpp"
#include <tuple>

class HealthDisplay : public ObserverInterface<std::tuple<float, jt::Vector2>> {
public:
    explicit HealthDisplay(std::shared_ptr<jt::Bar> bar);
    void notify(std::tuple<float, jt::Vector2> value) override;

private:
    std::shared_ptr<jt::Bar> m_bar;
};

#endif // QUASARRUSH_HEALTH_DISPLAY_HPP
