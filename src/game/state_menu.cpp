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
#include "state_game.hpp"
#include "text.hpp"
#include "tween_alpha.hpp"
#include "tween_position.hpp"
#include <fstream>

StateMenu::StateMenu() = default;

void StateMenu::doInternalCreate()
{
    createMenuText();

    createLevelButtons();
    createShapes();
    createVignette();
    createTweens();
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

        float xPos = 74.0f;
        float yPos = 80.0f + 24.0f * (int)(counter / 2);
        if (counter % 2 == 1) {
            xPos += 134.0f;
        }

        auto button = std::make_shared<jt::Button>(jt::Vector2u { 128, 18 });
        add(button);
        if (m_playerConfig.availableLevels.find(counter) == m_playerConfig.availableLevels.end()) {
            //button->setVisible(false);
            button->setActive(false);
        }
        button->addCallback([this, filename]() { startTransitionToStateGame(filename); });
        button->setPosition(jt::Vector2 { xPos, yPos });
        auto const text = jt::dh::createText(getGame()->getRenderTarget(), displayName, 12);
        text->setOrigin(jt::Vector2 { -4.0f, -2.0f });
        text->SetTextAlign(jt::Text::TextAlign::LEFT);
        button->setDrawable(text);

        m_buttons.push_back(button);

        counter++;
    }
}

void StateMenu::createVignette()
{
    m_vignette = jt::dh::createVignette(GP::GetScreenSize());
    m_vignette->setColor({ 255, 255, 255, 110 });
}

void StateMenu::createShapes()
{
    m_background = jt::dh::createRectShape(GP::GetScreenSize(), GP::PaletteColor1());
    m_overlay = jt::dh::createRectShape(GP::GetScreenSize(), jt::colors::Black);
}

void StateMenu::createMenuText()
{
    createTextTitle();
    createTextCredits();
}

void StateMenu::createTextCredits()
{
    m_text_Credits = jt::dh::createText(getGame()->getRenderTarget(),
        "Created by " + GP::AuthorName() + " for " + GP::JamName() + " " + GP::JamDate()
            + "\nCover 'QUASAR RUSH' by William Hackworth for Famicase2021"
            + "\nThis game uses OpenAl, licensed under LGPL v2.\nPlease see https://openal.org/ "
              "for details."
            + "\nThis game uses the font Oxanium, licensed under SIL OFL v1.1.",
        10U, GP::PaletteColor5());
    m_text_Credits->SetTextAlign(jt::Text::TextAlign::LEFT);
    m_text_Credits->setPosition({ 10, GP::GetScreenSize().y() - 68 });
    m_text_Credits->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 1, 1 });
}
void StateMenu::createTextTitle()
{
    float half_width = GP::GetScreenSize().x() / 2;
    m_text_Title = jt::dh::createText(
        getGame()->getRenderTarget(), GP::GameName(), 32U, GP::PaletteFontFront());
    m_text_Title->setPosition({ half_width, 20 });
    m_text_Title->setShadow(GP::PaletteFontShadow(), jt::Vector2 { 3, 3 });
}

void StateMenu::createTweens()
{
    createTweenOverlayAlpha();
    createTweenTitleAlpha();
    createTweenCreditsPosition();
}

void StateMenu::createTweenTitleAlpha()
{
    auto tween = jt::TweenAlpha<jt::Text>::create(m_text_Title, 0.55f, 0, 255);
    tween->setStartDelay(0.2f);
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::createTweenOverlayAlpha()
{
    auto tween = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::createTweenCreditsPosition()
{
    auto s3 = m_text_Credits->getPosition() + jt::Vector2 { 0, 100 };
    auto e3 = m_text_Credits->getPosition();

    auto tween = jt::TweenPosition<jt::Text>::create(m_text_Credits, 0.35f, s3, e3);
    tween->setStartDelay(0.8f);
    tween->setSkipFrames();
    add(tween);
}

void StateMenu::doInternalUpdate(float const elapsed) { updateDrawables(elapsed); }

void StateMenu::updateDrawables(const float& elapsed)
{
    m_background->update(elapsed);
    m_text_Title->update(elapsed);
    m_text_Credits->update(elapsed);
    m_overlay->update(elapsed);
    m_vignette->update(elapsed);
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
    auto tw = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.5f, std::uint8_t { 0 }, std::uint8_t { 255 });
    tw->setSkipFrames();
    tw->addCompleteCallback([this]() {
        std::shared_ptr<StateGame> newState = std::make_shared<StateGame>();
        newState->setLevel(m_levelFilename);
        newState->setPlayerConfig(m_playerConfig);
        getGame()->switchState(newState);
    });
    add(tw);
}

void StateMenu::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());

    m_text_Title->draw(getGame()->getRenderTarget());
    m_text_Credits->draw(getGame()->getRenderTarget());

    for (auto& button : m_buttons) {
        button->draw();
    }

    m_overlay->draw(getGame()->getRenderTarget());
    m_vignette->draw(getGame()->getRenderTarget());
}

void StateMenu::setPlayerConfig(PlayerConfig const& pc) { m_playerConfig = pc; }
