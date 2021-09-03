#ifndef SCORE_DISPLAY_GUARD
#define SCORE_DISPLAY_GUARD

#include "observer_interface.hpp"
#include "text.hpp"

class TimerDisplay : public ObserverInterface<float> {
public:
    explicit TimerDisplay(jt::Text::Sptr text, std::string const& prefix);
    void notify(float value) override;

private:
    jt::Text::Sptr const m_timerText;
    std::string m_textPrefix;
};

#endif // !SCORE_DISPLAY_GUARD
