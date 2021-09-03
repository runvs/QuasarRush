#include "hud.hpp"
#include "color.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "time_display.hpp"

std::shared_ptr<ObserverInterface<float>> Hud::getObserverTimer() const { return m_timeDisplay; }

void Hud::doCreate()
{
    m_timeText = std::make_shared<jt::Text>();
    m_timeText->loadFont("assets/font.ttf", 16, getGame()->getRenderTarget());
    m_timeText->setColor(jt::Color { 248, 249, 254 });
    m_timeText->update(0.0f);
    m_timeText->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_timeText->setPosition({ 10, 4 });

    m_timeDisplay = std::make_shared<TimerDisplay>(m_timeText, "");

}

void Hud::doUpdate(float const elapsed)
{
    m_timeText->update(elapsed);
}

void Hud::doDraw() const
{
    m_timeText->draw(getGame()->getRenderTarget());
}
