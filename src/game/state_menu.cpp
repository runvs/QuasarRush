#include "state_menu.hpp"
#include "button.hpp"
#include "color.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "input_manager.hpp"
#include "json.hpp"
#include "key_codes.hpp"
#include "math_helper.hpp"
#include "player_config.hpp"
#include "shape.hpp"
#include "state_credits.hpp"
#include "state_game.hpp"
#include "text.hpp"
#include "tween_alpha.hpp"
#include "tween_position.hpp"
#include <fstream>

StateMenu::StateMenu() = default;

void StateMenu::doInternalCreate()
{
    m_menuBase = std::make_shared<StateMenuBase>();
    m_menuBase->create(getGame()->getRenderTarget(), *this);

    createLevelButtons();

    m_buttonCredits = std::make_shared<jt::Button>(jt::Vector2u { 51, 18 });
    add(m_buttonCredits);
    m_buttonCredits->addCallback([this]() {
        auto newState = std::make_shared<StateCredits>();
        getGame()->switchState(newState);
    });
    m_buttonCredits->setPosition(jt::Vector2 { 10, 280 });
    auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Credits", 12);
    text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
    text->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_buttonCredits->setDrawable(text);

    getGame()->getRenderWindow()->setMouseCursorVisible(true);

    createShipUpgradeButtons();
}
void StateMenu::createShipUpgradeButtons()
{
    float const increment = 24.0f;
    float const xOffset = 280.0f;
    float const yOffset = 186.0f;

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
            [this]() { playerConfigIncreaseSensors(m_playerConfig); });
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
            [this]() { playerConfigIncreaseEngine(m_playerConfig); });
        m_buttonIncreaseEngine->setPosition(jt::Vector2 { xOffset, yOffset + 1.0f * increment });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "+", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonIncreaseEngine->setDrawable(text);
    }

    {
        m_buttonSwitchToMG = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonSwitchToMG);

        m_buttonSwitchToMG->addCallback([this]() { playerConfigSwitchToMg(m_playerConfig); });
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
            [this]() { playerConfigSwitchToMissile(m_playerConfig); });
        m_buttonSwitchToMissile->setPosition(
            jt::Vector2 { xOffset + 2.0f * increment, yOffset + 24 });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Ro", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonSwitchToMissile->setDrawable(text);
    }
}
void StateMenu::createLevelButtons()
{
    nlohmann::json j;
    std::ifstream file { "assets/levels/__all_levels.json" };
    file >> j;

    auto levels = j["levels"];
    int counter = 0;

    for (auto l : levels) {
        std::string filename = l["filename"];
        std::string displayName = l["display"];

        float xPos = 10.0f;
        float yPos = 125.0f + 36.0f + 24.0f * (int)(counter / 2);
        unsigned int buttonWidth = 100U;
        if (counter % 2 == 1) {
            xPos += buttonWidth + 8.0f;
        }

        auto button = std::make_shared<jt::Button>(jt::Vector2u { buttonWidth, 18 });
        add(button);
        if (m_playerConfig.availableLevels.find(counter) == m_playerConfig.availableLevels.end()) {
            // button->setVisible(false);
            button->setActive(false);
        }
        button->addCallback([this, filename]() { startTransitionToStateGame(filename); });
        button->setPosition(jt::Vector2 { xPos, yPos });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), displayName, 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        button->setDrawable(text);

        m_buttons.push_back(button);

        counter++;
    }
}


void StateMenu::doInternalUpdate(float const elapsed)
{
    updateDrawables(elapsed);
    updateShipUpgradeButtons(elapsed);
}
void StateMenu::updateShipUpgradeButtons(float const elapsed)
{
    m_buttonIncreaseSensors->setActive(playerConfigHasPointsToSpend(m_playerConfig));
    m_buttonIncreaseEngine->setActive(playerConfigHasPointsToSpend(m_playerConfig));

    m_buttonSwitchToMG->setActive(playerConfigCanSwitchToMg(m_playerConfig));
    m_buttonSwitchToMissile->setActive(playerConfigCanSwitchToMissile(m_playerConfig));

    m_textEngine->update(elapsed);
    m_textSensor->update(elapsed);
}

void StateMenu::updateDrawables(const float& elapsed)
{
    m_menuBase->update(elapsed);
}

void StateMenu::startTransitionToStateGame(std::string const& levelFilename)
{
    if (!m_started) {
        m_started = true;
        m_levelFilename = levelFilename;
        createTweenTransition();
    }
}

void StateMenu::createTweenTransition()
{
    m_menuBase->startFadeOut([this]() {
        std::shared_ptr<StateGame> newState = std::make_shared<StateGame>();
        newState->setLevel(m_levelFilename);
        newState->setPlayerConfig(m_playerConfig);
        getGame()->switchState(newState);
    }, *this);
}

void StateMenu::doInternalDraw() const
{
    m_menuBase->draw(getGame()->getRenderTarget());

    for (auto& button : m_buttons) {
        button->draw();
    }
    m_buttonCredits->draw();
    m_buttonIncreaseSensors->draw();
    m_textEngine->draw(getGame()->getRenderTarget());
    m_textSensor->draw(getGame()->getRenderTarget());
    m_buttonIncreaseEngine->draw();
    m_buttonSwitchToMG->draw();
    m_buttonSwitchToMissile->draw();

    m_menuBase->drawOverlay(getGame()->getRenderTarget());
}

void StateMenu::setPlayerConfig(PlayerConfig const& pc) { m_playerConfig = pc; }
