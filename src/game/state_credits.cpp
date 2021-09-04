#include "state_credits.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "state_menu.hpp"
#include "text.hpp"
#include <tween_position.hpp>

StateCredits::StateCredits() { m_menuBase = std::make_shared<StateMenuBase>(); }

void StateCredits::doInternalCreate()
{

    m_menuBase->create(getGame()->getRenderTarget(), *this);

    m_text_Credits = jt::dh::createText(getGame()->getRenderTarget(),
        "Created by " + GP::AuthorName() + " for " + GP::JamName() + " in " + GP::JamDate()
            + "\nCover 'QUASAR RUSH' by William Hackworth for Famicase2021"
            + "\nThis game uses OpenAl, licensed under LGPL v2. https://openal.org/"
            + "\nThis game uses the font Oxanium, licensed under SIL OFL v1.1.",
        10U, GP::PaletteFontFront());
    m_text_Credits->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_text_Credits->setPosition(
        { 10, GP::GetScreenSize().y() - m_text_Credits->getLocalBounds().height() - 8 });
    m_text_Credits->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 1, 1 });

    auto s3 = m_text_Credits->getPosition() + jt::Vector2 { 0, 100 };
    auto e3 = m_text_Credits->getPosition();

    auto tween = jt::TweenPosition<jt::Text>::create(m_text_Credits, 0.35f, s3, e3);
    tween->setStartDelay(0.8f);
    tween->setSkipFrames();
    add(tween);
}
void StateCredits::doInternalUpdate(float elapsed)
{
    m_menuBase->update(elapsed);
    m_text_Credits->update(elapsed);

    if (getGame()->input()->keyboard()->justPressed(jt::KeyCode::Escape)) {
        startTransitionToStateMenu();
    }
}

void StateCredits::startTransitionToStateMenu()
{
    if (!m_started) {
        m_started = true;
        createTweenTransition();
    }
}

void StateCredits::doInternalDraw() const
{
    m_menuBase->draw(getGame()->getRenderTarget());
    m_text_Credits->draw(getGame()->getRenderTarget());
    m_menuBase->drawOverlay(getGame()->getRenderTarget());
}
void StateCredits::setPlayerConfig(PlayerConfig const& pc) { m_menuBase->m_playerConfig = pc; }
void StateCredits::createTweenTransition()
{
    m_menuBase->startFadeOut(
        [this]() {
            std::shared_ptr<StateMenu> newState = std::make_shared<StateMenu>();
            newState->setPlayerConfig(m_menuBase->m_playerConfig);
            getGame()->switchState(newState);
        },
        *this);
}
