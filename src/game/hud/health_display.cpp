#include "health_display.hpp"
#include <math_helper.hpp>

HealthDisplay::HealthDisplay(std::shared_ptr<jt::Bar> bar, std::shared_ptr<jt::Sprite> overlay)
    : m_bar { bar }
    , m_overlay { overlay }
{
}

void HealthDisplay::notify(std::tuple<float, jt::Vector2> value)
{
    auto const hp = std::get<0>(value);
    auto const pos = std::get<1>(value);

    if (m_bar->getMaxValue() <= hp) {
        m_bar->setMaxValue(hp);
    }
    m_bar->setPosition(pos + jt::Vector2 { -8.0f, -12.0f });
    m_bar->setCurrentValue(hp);
    m_bar->update(0.1f);

    auto c = m_overlay->getColor();
    if (hp > 3) {
        c.a() = 0;
    } else {

        std::uint8_t v = jt::MathHelper::clamp(static_cast<std::uint8_t>(3 - hp) * 80U, 0U, 160U);
        c.a() = v;
    }
    m_overlay->setColor(c);
}
