#include "health_display.hpp"

HealthDisplay::HealthDisplay(std::shared_ptr<jt::Bar> bar)
    : m_bar { bar }
{
}

void HealthDisplay::notify(std::tuple<float, jt::Vector2> value) {
    auto const hp = std::get<0>(value);
    auto const pos = std::get<1>(value);

    if (m_bar->getMaxValue() <= hp)
    {
        m_bar->setMaxValue(hp);
    }
    m_bar->setPosition(pos + jt::Vector2{-8.0f, -12.0f});
    m_bar->setCurrentValue(hp);
    m_bar->update(0.1f);
}
