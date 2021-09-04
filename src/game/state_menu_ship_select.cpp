#include "state_menu_ship_select.hpp"
#include "button.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "state_game.hpp"
#include "state_menu_level_select.hpp"
#include "text.hpp"

StateMenuShipSelect::StateMenuShipSelect() { m_menuBase = std::make_shared<StateMenuBase>(); }

void StateMenuShipSelect::setPlayerConfig(PlayerConfig const& pc)
{
    m_menuBase->m_playerConfig = pc;
}

void StateMenuShipSelect::doInternalCreate()
{
    m_menuBase->create(getGame()->getRenderTarget(), *this);

    createShipUpgradeButtons();

    createButtonBack();
    createButtonFly();

    m_textPointsAvailable
        = jt::dh::createText(getGame()->getRenderTarget(), "Points: 0", 12U, GP::PaletteFontFront());
    m_textPointsAvailable->setPosition(jt::Vector2 { 170, 200 });
}

void StateMenuShipSelect::createButtonBack()
{
    m_buttonBack = std::make_shared<jt::Button>(jt::Vector2u { 51, 18 });
    add(m_buttonBack);
    m_buttonBack->addCallback([this]() {
        auto newState = std::make_shared<StateMenuLevelSelect>();
        newState->setPlayerConfig(m_menuBase->m_playerConfig);
        getGame()->switchState(newState);
    });
    m_buttonBack->setPosition(jt::Vector2 { 10, 278 });
    auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Back", 12);
    text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
    text->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_buttonBack->setDrawable(text);
}

void StateMenuShipSelect::createButtonFly()
{
    m_buttonFly = std::make_shared<jt::Button>(jt::Vector2u { 51, 18 });
    add(m_buttonFly);
    m_buttonFly->addCallback([this]() {
        startTransitionToStateGame();
    });
    m_buttonFly->setPosition(jt::Vector2 { 100, 278 });
    auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Fly", 12);
    text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
    text->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_buttonFly->setDrawable(text);
}

void StateMenuShipSelect::doInternalUpdate(float const elapsed)
{
    m_menuBase->update(elapsed);
    m_buttonBack->update(elapsed);

    m_textSensor->update(elapsed);
    m_textEngine->update(elapsed);
    m_textPointsAvailable->setText("Points: " + std::to_string(m_menuBase->m_playerConfig.pointsToSpend));
    m_textPointsAvailable->update(elapsed);
    updateShipUpgradeButtons(elapsed);
}

void StateMenuShipSelect::doInternalDraw() const
{

    m_menuBase->draw(getGame()->getRenderTarget());

    m_buttonBack->draw();
    m_buttonFly->draw();

    m_buttonIncreaseSensors->draw();
    m_textSensor->draw(getGame()->getRenderTarget());
    m_textEngine->draw(getGame()->getRenderTarget());
    m_buttonIncreaseEngine->draw();
    m_buttonSwitchToMG->draw();
    m_buttonSwitchToMissile->draw();

    m_textPointsAvailable->draw(getGame()->getRenderTarget());

    m_menuBase->drawOverlay(getGame()->getRenderTarget());
}

void StateMenuShipSelect::startTransitionToStateGame()
{
    if (!m_started) {
        m_started = true;
        createTweenTransition();
    }
}
void StateMenuShipSelect::createTweenTransition()
{
    m_menuBase->startFadeOut(
        [this]() {
            std::shared_ptr<StateGame> newState = std::make_shared<StateGame>();
            newState->setPlayerConfig(m_menuBase->m_playerConfig);
            getGame()->switchState(newState);
        },
        *this);
}
void StateMenuShipSelect::createShipUpgradeButtons()
{
    float const increment = 24.0f;
    float const xOffset = 280.0f;
    float const yOffset = 200.0f;

    m_textSensor
        = jt::dh::createText(getGame()->getRenderTarget(), "Sensors", 12U, GP::PaletteFontFront());
    m_textSensor->setPosition(jt::Vector2 { xOffset - 30, yOffset + 0 * increment });

    m_textEngine
        = jt::dh::createText(getGame()->getRenderTarget(), "Engine", 12U, GP::PaletteFontFront());
    m_textEngine->setPosition(jt::Vector2 { xOffset - 27, yOffset + 1.0f * increment });

    {
        m_buttonIncreaseSensors = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonIncreaseSensors);
        m_buttonIncreaseSensors->addCallback(
            [this]() { playerConfigIncreaseSensors(m_menuBase->m_playerConfig); });
        m_buttonIncreaseSensors->setPosition(jt::Vector2 { xOffset, yOffset + 0 * increment });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "+", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonIncreaseSensors->setDrawable(text);
    }
    {
        m_buttonIncreaseEngine = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonIncreaseEngine);
        m_buttonIncreaseEngine->addCallback(
            [this]() { playerConfigIncreaseEngine(m_menuBase->m_playerConfig); });
        m_buttonIncreaseEngine->setPosition(jt::Vector2 { xOffset, yOffset + 1.0f * increment });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "+", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonIncreaseEngine->setDrawable(text);
    }

    {
        m_buttonSwitchToMG = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonSwitchToMG);

        m_buttonSwitchToMG->addCallback(
            [this]() { playerConfigSwitchToMg(m_menuBase->m_playerConfig); });
        m_buttonSwitchToMG->setPosition(
            jt::Vector2 { xOffset + 2 * increment, yOffset + 0 * increment });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Mg", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonSwitchToMG->setDrawable(text);
    }
    {
        m_buttonSwitchToMissile = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonSwitchToMissile);

        m_buttonSwitchToMissile->addCallback(
            [this]() { playerConfigSwitchToMissile(m_menuBase->m_playerConfig); });
        m_buttonSwitchToMissile->setPosition(
            jt::Vector2 { xOffset + 2.0f * increment, yOffset + 24 });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Ro", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonSwitchToMissile->setDrawable(text);
    }
}
void StateMenuShipSelect::updateShipUpgradeButtons(float const elapsed)
{
    m_buttonIncreaseSensors->setActive(playerConfigHasPointsToSpend(m_menuBase->m_playerConfig));
    m_buttonIncreaseEngine->setActive(playerConfigHasPointsToSpend(m_menuBase->m_playerConfig));

    m_buttonSwitchToMG->setActive(playerConfigCanSwitchToMg(m_menuBase->m_playerConfig));
    m_buttonSwitchToMissile->setActive(playerConfigCanSwitchToMissile(m_menuBase->m_playerConfig));

    m_textEngine->update(elapsed);
    m_textSensor->update(elapsed);
}
