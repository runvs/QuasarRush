#include "reload_display.hpp"

ReloadDisplay::ReloadDisplay(std::shared_ptr<jt::Bar> bar)
    : m_bar { bar }
{
    bar->setMaxValue(1.0f);
}

void ReloadDisplay::notify(float value)
{

    if (m_bar->getValueFraction() != 0) {
        if (value == 0)
        {
            m_bar->flash(0.5f, jt::colors::Green);
        }
    }
    m_bar->setCurrentValue(value);
}
