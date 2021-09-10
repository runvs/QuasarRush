#include "state_menu_ship_select.hpp"
#include "button.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "state_game.hpp"
#include "state_menu_stage_select.hpp"
#include "text.hpp"

StateMenuShipSelect::StateMenuShipSelect() { m_menuBase = std::make_shared<StateMenuBase>(); }

void StateMenuShipSelect::setPlayerConfig(PlayerConfig const& pc)
{
    m_menuBase->m_playerConfig = pc;
}

void StateMenuShipSelect::doInternalCreate()
{
    m_menuBase->create(getGame()->getRenderTarget(), *this, true);
    m_menuBase->setSubtitleText("Hangar");

    createShipUpgradeButtons();

    createButtonBack();
    createWeaponButtons();
    createButtonFly();

    createInfoTexts();

    m_textPointsAvailable = jt::dh::createText(
        getGame()->getRenderTarget(), "Points: 0", 12U, GP::PaletteFontFront());
    m_textPointsAvailable->setPosition(jt::Vector2 { 100, 180 });
}
void StateMenuShipSelect::createInfoTexts()
{
    jt::Vector2 const& InfoTextOffset = jt::Vector2 { 4, -3.0f };

    m_infoTextSensors = std::make_shared<InfoText>(
        m_buttonIncreaseSensors->getAnimation(), "Better Prediction", RightDown);

    m_infoTextSensors->setOffset(InfoTextOffset);
    add(m_infoTextSensors);

    m_infoTextEngine = std::make_shared<InfoText>(
        m_buttonIncreaseEngine->getAnimation(), "Faster Ship", RightDown);
    m_infoTextEngine->setOffset(InfoTextOffset);
    add(m_infoTextEngine);

    m_infoTextWeapon = std::make_shared<InfoText>(
        m_buttonIncreaseWeapon->getAnimation(), "Reload Speed", RightDown);
    m_infoTextWeapon->setOffset(InfoTextOffset);
    add(m_infoTextWeapon);

    m_infoTextHull
        = std::make_shared<InfoText>(m_buttonIncreaseHull->getAnimation(), "More Armor", RightDown);
    m_infoTextHull->setOffset(InfoTextOffset);
    add(m_infoTextHull);

    m_infoTextMg
        = std::make_shared<InfoText>(m_buttonSwitchToMg->getAnimation(), "Machinegun", LeftDown);
    m_infoTextMg->setOffset(InfoTextOffset);
    add(m_infoTextMg);

    m_infoTextRockets
        = std::make_shared<InfoText>(m_buttonSwitchToRockets->getAnimation(), "Rockets", LeftDown);
    m_infoTextRockets->setOffset(InfoTextOffset);
    add(m_infoTextRockets);
}

void StateMenuShipSelect::createButtonBack()
{
    m_buttonBack = std::make_shared<jt::Button>(jt::Vector2u { 51, 18 });
    add(m_buttonBack);
    m_buttonBack->addCallback([this]() {
        auto newState = std::make_shared<StateMenuStageSelect>();
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
    m_buttonFly->addCallback([this]() { startTransitionToStateGame(); });
    m_buttonFly->setPosition(jt::Vector2 { 100, 278 });
    auto const text = jt::dh::createText(getGame()->getRenderTarget(), "Fly", 12);
    text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
    text->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_buttonFly->setDrawable(text);
}

void StateMenuShipSelect::createShipUpgradeButtons()
{

    float const xOffset = 100.0f;
    float const yOffset = 210.0f;

    float const xIncrementButton = 30.0f;
    float const xIncrementText = 132.0f;
    float const yIncrement = 30.0f;

    float const textXOffset = 35.5;

    m_textSensor
        = jt::dh::createText(getGame()->getRenderTarget(), "Sensors", 12U, GP::PaletteFontFront());
    m_textSensor->setPosition(jt::Vector2 { xOffset - textXOffset, yOffset + 0 * yIncrement });

    m_textEngine
        = jt::dh::createText(getGame()->getRenderTarget(), "Engine", 12U, GP::PaletteFontFront());
    m_textEngine->setPosition(
        jt::Vector2 { xOffset - textXOffset + 5, yOffset + 1.0f * yIncrement });

    m_textWeapon
        = jt::dh::createText(getGame()->getRenderTarget(), "Weapon", 12U, GP::PaletteFontFront());
    m_textWeapon->setPosition(
        jt::Vector2 { xOffset - textXOffset - 2 + xIncrementText, yOffset + 0.0f * yIncrement });

    m_textHull
        = jt::dh::createText(getGame()->getRenderTarget(), "Hull", 12U, GP::PaletteFontFront());
    m_textHull->setPosition(
        jt::Vector2 { xOffset - textXOffset - 16 + xIncrementText, yOffset + 1.0f * yIncrement });

    {
        m_buttonIncreaseSensors = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonIncreaseSensors);
        m_buttonIncreaseSensors->addCallback(
            [this]() { playerConfigIncreaseSensors(m_menuBase->m_playerConfig); });
        m_buttonIncreaseSensors->setPosition(jt::Vector2 { xOffset, yOffset + 0 * yIncrement });
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
        m_buttonIncreaseEngine->setPosition(jt::Vector2 { xOffset, yOffset + 1.0f * yIncrement });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "+", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonIncreaseEngine->setDrawable(text);
    }
    {
        m_buttonIncreaseWeapon = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonIncreaseWeapon);
        m_buttonIncreaseWeapon->addCallback(
            [this]() { playerConfigIncreaseWeapon(m_menuBase->m_playerConfig); });
        m_buttonIncreaseWeapon->setPosition(
            jt::Vector2 { xOffset + xIncrementButton, yOffset + 0.0f * yIncrement });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "+", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonIncreaseWeapon->setDrawable(text);
    }
    {
        m_buttonIncreaseHull = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonIncreaseHull);
        m_buttonIncreaseHull->addCallback(
            [this]() { playerConfigIncreaseHull(m_menuBase->m_playerConfig); });
        m_buttonIncreaseHull->setPosition(
            jt::Vector2 { xOffset + xIncrementButton, yOffset + 1.0f * yIncrement });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), "+", 12);
        text->setOrigin(jt::Vector2 { -5.0f, -1.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        m_buttonIncreaseHull->setDrawable(text);
    }
}
void StateMenuShipSelect::createWeaponButtons()
{
    float const increment = 30.0f;
    float const xOffset = 250.0f;
    float const yOffset = 210.0f;
    {
        m_buttonSwitchToMg = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonSwitchToMg);

        m_buttonSwitchToMg->addCallback(
            [this]() { playerConfigSwitchToMg(m_menuBase->m_playerConfig); });
        m_buttonSwitchToMg->setPosition(
            jt::Vector2 { xOffset + 2 * increment, yOffset + 0 * increment });
        auto const img = std::make_shared<jt::Sprite>();
        img->loadSprite("assets/icons/mg.png");
        img->setOrigin(jt::Vector2 { -1.0f, -1.0f });

        m_buttonSwitchToMg->setDrawable(img);
    }
    {
        m_buttonSwitchToRockets = std::make_shared<jt::Button>(jt::Vector2u { 18, 18 });
        add(m_buttonSwitchToRockets);

        m_buttonSwitchToRockets->addCallback(
            [this]() { playerConfigSwitchToMissile(m_menuBase->m_playerConfig); });
        m_buttonSwitchToRockets->setPosition(
            jt::Vector2 { xOffset + 2.0f * increment, yOffset + increment });
        auto const img = std::make_shared<jt::Sprite>();
        img->loadSprite("assets/icons/rocket.png");
        img->setOrigin(jt::Vector2 { -1.0f, -1.0f });
        m_buttonSwitchToRockets->setDrawable(img);
    }
}

void StateMenuShipSelect::doInternalUpdate(float const elapsed)
{
    m_menuBase->update(elapsed);
    m_buttonBack->update(elapsed);

    updateTexts(elapsed);

    infoTextUpdate();

    updateShipUpgradeButtons(elapsed);

    if (getGame()->input()->keyboard()->pressed(jt::KeyCode::LShift)
        && getGame()->input()->keyboard()->justPressed(jt::KeyCode::F8)) {
        m_menuBase->m_playerConfig.pointsToSpend++;
    }
}

void updateInfoText(std::shared_ptr<InfoText> text, std::shared_ptr<jt::Button> button)
{
    if (button->IsMouseOver()) {
        text->setColor(jt::Color { 255, 255, 255 });
    } else {
        text->setColor(jt::Color { 255, 255, 255, 0 });
    }
}

void StateMenuShipSelect::infoTextUpdate()
{
    updateInfoText(m_infoTextSensors, m_buttonIncreaseSensors);
    updateInfoText(m_infoTextEngine, m_buttonIncreaseEngine);
    updateInfoText(m_infoTextWeapon, m_buttonIncreaseWeapon);
    updateInfoText(m_infoTextHull, m_buttonIncreaseHull);

    updateInfoText(m_infoTextMg, m_buttonSwitchToMg);
    updateInfoText(m_infoTextRockets, m_buttonSwitchToRockets);
}
void StateMenuShipSelect::updateTexts(float const elapsed)
{
    m_textSensor->setText("Sensors: " + std::to_string(m_menuBase->m_playerConfig.sensorLevel));
    m_textSensor->update(elapsed);

    m_textEngine->setText("Engine: " + std::to_string(m_menuBase->m_playerConfig.engineLevel));
    m_textEngine->update(elapsed);

    m_textWeapon->setText(std::to_string(m_menuBase->m_playerConfig.weaponLevel) + " :Weapons");
    m_textWeapon->update(elapsed);

    m_textHull->setText(std::to_string(m_menuBase->m_playerConfig.hullLevel) + " :Hull");
    m_textHull->update(elapsed);

    m_textPointsAvailable->setText(
        "Points: " + std::to_string(m_menuBase->m_playerConfig.pointsToSpend));
    m_textPointsAvailable->update(elapsed);
}

void StateMenuShipSelect::doInternalDraw() const
{
    m_menuBase->draw(getGame()->getRenderTarget());

    m_buttonBack->draw();
    m_buttonFly->draw();

    m_textSensor->draw(getGame()->getRenderTarget());
    m_buttonIncreaseSensors->draw();

    m_textEngine->draw(getGame()->getRenderTarget());
    m_buttonIncreaseEngine->draw();

    m_textWeapon->draw(getGame()->getRenderTarget());
    m_buttonIncreaseWeapon->draw();

    m_textHull->draw(getGame()->getRenderTarget());
    m_buttonIncreaseHull->draw();

    m_buttonSwitchToMg->draw();
    m_buttonSwitchToRockets->draw();

    m_textPointsAvailable->draw(getGame()->getRenderTarget());

    m_infoTextSensors->draw();
    m_infoTextEngine->draw();
    m_infoTextWeapon->draw();
    m_infoTextHull->draw();
    m_infoTextMg->draw();
    m_infoTextRockets->draw();

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

void StateMenuShipSelect::updateShipUpgradeButtons(float const /*elapsed*/)
{
    bool const hasPointsToSpend = playerConfigHasPointsToSpend(m_menuBase->m_playerConfig);
    m_buttonIncreaseSensors->setActive(hasPointsToSpend);
    m_buttonIncreaseEngine->setActive(hasPointsToSpend);
    m_buttonIncreaseWeapon->setActive(hasPointsToSpend);
    m_buttonIncreaseHull->setActive(hasPointsToSpend);

    m_buttonSwitchToMg->setActive(playerConfigCanSwitchToMg(m_menuBase->m_playerConfig));
    m_buttonSwitchToRockets->setActive(playerConfigCanSwitchToMissile(m_menuBase->m_playerConfig));
}
