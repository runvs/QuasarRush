#include "hud.hpp"
#include "color.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "time_display.hpp"
#include "reload_display.hpp"
#include "health_display.hpp"

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

    m_reloadBar = std::make_shared<jt::Bar>(4.0f, 16.0f, false);
    m_reloadBar->setFrontColor(jt::colors::White);
    m_reloadDisplay = std::make_shared<ReloadDisplay>(m_reloadBar);

    m_healthBar = std::make_shared<jt::Bar>(16.0f, 2.0f, true);
    m_healthBar->setFrontColor(jt::Color{95,5,100});
    m_healthOverlay = std::make_shared<jt::Sprite>();
    m_healthOverlay->loadSprite("assets/vignette.png");
    m_healthDisplay = std::make_shared<HealthDisplay>(m_healthBar, m_healthOverlay);
}

void Hud::doUpdate(float const elapsed)
{
    m_timeText->update(elapsed);
    m_reloadBar->setPosition(getGame()->input()->mouse()->getMousePositionWorld() + jt::Vector2{16.0f, -12.0f});
    m_reloadBar->update(elapsed);
    m_healthBar->update(elapsed);
    m_healthOverlay->update(elapsed);
}

void Hud::doDraw() const
{
    m_timeText->draw(getGame()->getRenderTarget());
    m_reloadBar->draw(getGame()->getRenderTarget());
    m_healthBar->draw(getGame()->getRenderTarget());
    m_healthOverlay->draw(getGame()->getRenderTarget());
}

std::shared_ptr<ObserverInterface<float>> Hud::getObserverReload() const
{
    return m_reloadDisplay;
}

std::shared_ptr<ObserverInterface<std::tuple<float,jt::Vector2>>>  Hud::getObserverHealth() const
{
    return m_healthDisplay;
}
