#include "time_display.hpp"
#include <sstream>
#include <iomanip>

TimerDisplay::TimerDisplay(jt::Text::Sptr text, std::string const& prefix)
    : m_timerText { text }
    , m_textPrefix { prefix }
{
}

void TimerDisplay::notify(float value)
{
    if (value >= 0) {
        std::stringstream ss;
        ss.precision(1);
        ss.setf(std::ios::fixed, std::ios::floatfield);
        ss << value;
        m_timerText->setText(m_textPrefix + ss.str());
    }
}
