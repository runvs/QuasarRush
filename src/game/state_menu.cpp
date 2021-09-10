#include "state_menu.hpp"
#include "button.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "input_manager.hpp"
#include "json.hpp"
#include "key_codes.hpp"
#include "math_helper.hpp"
#include "state_menu_credits.hpp"
#include "state_menu_stage_select.hpp"
#include "text.hpp"
#include <fstream>
#include "tween_alpha.hpp"

StateMenu::StateMenu() { m_menuBase = std::make_shared<StateMenuBase>(); }

void StateMenu::setPlayerConfig(PlayerConfig const& pc) { m_menuBase->m_playerConfig = pc; }

void StateMenu::doInternalCreate()
{
    m_menuBase->create(getGame()->getRenderTarget(), *this, true);
    m_menuBase->setSubtitleText("A Gravity Combat Game");
    getGame()->getRenderWindow()->setMouseCursorVisible(true);

    createButtonCredits();
    createButtonSelectLevel();
}
void StateMenu::createButtonSelectLevel()
{
    m_buttonSelectLevel = std::make_shared<jt::Button>(jt::Vector2u { 35, 18 });
    add(m_buttonSelectLevel);
    m_buttonSelectLevel->addCallback([this]() { startTransitionToStateSelectLevel(); });
    m_buttonSelectLevel->setPosition(jt::Vector2 { 185, 200 });
    // TODO "Select Stage" ?
    auto const text2 = jt::dh::createText(getGame()->getRenderTarget(), "Play", 12);
    text2->setOrigin(jt::Vector2 { -5.0f, -1.0f });
    text2->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_buttonSelectLevel->setDrawable(text2);
}
void StateMenu::createButtonCredits()
{
    m_buttonCredits = std::make_shared<jt::Button>(jt::Vector2u { 51, 18 });
    add(m_buttonCredits);
    m_buttonCredits->addCallback([this]() {
        auto newState = std::make_shared<StateMenuCredits>();
        newState->setPlayerConfig(m_menuBase->m_playerConfig);
        getGame()->switchState(newState);
    });
    m_buttonCredits->setPosition(jt::Vector2 { 345, 278 });
    auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Credits", 12);
    text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
    text->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_buttonCredits->setDrawable(text);
}

void StateMenu::doInternalUpdate(float const elapsed) { m_menuBase->update(elapsed); }

void StateMenu::startTransitionToStateSelectLevel()
{
    if (!m_started) {
        m_started = true;
        createTweenTransition();
    }
}

void StateMenu::createTweenTransition()
{
    m_menuBase->startFadeOut(
        [this]() {
            std::shared_ptr<StateMenuStageSelect> newState
                = std::make_shared<StateMenuStageSelect>();
            newState->setPlayerConfig(m_menuBase->m_playerConfig);
            getGame()->switchState(newState);
        },
        *this);
}

void StateMenu::doInternalDraw() const
{
    m_menuBase->draw(getGame()->getRenderTarget());

    m_buttonCredits->draw();
    m_buttonSelectLevel->draw();

    m_menuBase->drawOverlay(getGame()->getRenderTarget());
}
