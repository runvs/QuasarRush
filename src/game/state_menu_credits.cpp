#include "state_menu_credits.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "state_menu.hpp"
#include "text.hpp"
#include "button.hpp"
#include <tween_position.hpp>

StateMenuCredits::StateMenuCredits() { m_menuBase = std::make_shared<StateMenuBase>(); }

void StateMenuCredits::doInternalCreate()
{
    m_menuBase->create(getGame()->getRenderTarget(), *this, false);
    m_menuBase->setSubtitleText("Credits");

    createCreditsText();
    createButtonBack();
}
void StateMenuCredits::createCreditsText()
{
    m_text_Credits = jt::dh::createText(getGame()->getRenderTarget(),
        "Created by " + GP::AuthorName() + " for " + GP::JamName() + " in " + GP::JamDate()
            + "\nCover 'QUASAR RUSH' by William Hackworth for Famicase2021"
            + "\nThis game uses OpenAl, licensed under LGPL v2. https://openal.org/"
            + "\nThis game uses the font Oxanium, licensed under SIL OFL v1.1.",
        12U, GP::PaletteFontFront());
    m_text_Credits->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_text_Credits->setPosition(
        { 10, 200.0f });
    m_text_Credits->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 1, 1 });

    auto s3 = m_text_Credits->getPosition() + jt::Vector2 { 0, 100 };
    auto e3 = m_text_Credits->getPosition();

    auto tween = jt::TweenPosition<jt::Text>::create(m_text_Credits, 0.35f, s3, e3);
    tween->setSkipFrames();
    add(tween);
}

void StateMenuCredits::createButtonBack()
{
    m_buttonBack = std::make_shared<jt::Button>(jt::Vector2u { 51, 18 });
    add(m_buttonBack);
    m_buttonBack->addCallback([this]() {
        auto newState = std::make_shared<StateMenu>();
        newState->setPlayerConfig(m_menuBase->m_playerConfig);
        getGame()->switchState(newState);
    });
    m_buttonBack->setPosition(jt::Vector2 { 10, 278 });
    auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Back", 12);
    text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
    text->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_buttonBack->setDrawable(text);
}

void StateMenuCredits::doInternalUpdate(float elapsed)
{
    m_menuBase->update(elapsed);
    m_text_Credits->update(elapsed);

    if (getGame()->input()->keyboard()->justPressed(jt::KeyCode::Escape)) {
        startTransitionToStateMenu();
    }
}

void StateMenuCredits::startTransitionToStateMenu()
{
    if (!m_started) {
        m_started = true;
        createTweenTransition();
    }
}

void StateMenuCredits::doInternalDraw() const
{
    m_menuBase->draw(getGame()->getRenderTarget());
    m_text_Credits->draw(getGame()->getRenderTarget());
    m_buttonBack->draw();
    m_menuBase->drawOverlay(getGame()->getRenderTarget());

}

void StateMenuCredits::setPlayerConfig(PlayerConfig const& pc) { m_menuBase->m_playerConfig = pc; }

void StateMenuCredits::createTweenTransition()
{
    m_menuBase->startFadeOut(
        [this]() {
            std::shared_ptr<StateMenu> newState = std::make_shared<StateMenu>();
            newState->setPlayerConfig(m_menuBase->m_playerConfig);
            getGame()->switchState(newState);
        },
        *this);
}
