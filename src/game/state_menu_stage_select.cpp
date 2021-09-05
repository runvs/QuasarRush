#include "state_menu_stage_select.hpp"
#include "button.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "state_menu.hpp"
#include "state_menu_ship_select.hpp"
#include "text.hpp"
#include "vector.hpp"
#include <fstream>
#include <json.hpp>

StateMenuStageSelect::StateMenuStageSelect() { m_menuBase = std::make_shared<StateMenuBase>(); }

void StateMenuStageSelect::doInternalCreate()
{
    m_menuBase->create(getGame()->getRenderTarget(), *this);
    m_menuBase->setSubtitleText("Stage Selection");
    getGame()->getRenderWindow()->setMouseCursorVisible(true);

    createLevelButtons();
    createButtonBack();
}
void StateMenuStageSelect::createButtonBack()
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

void StateMenuStageSelect::createTweenTransition()
{
    m_menuBase->startFadeOut(
        [this]() {
            std::shared_ptr<StateMenuShipSelect> newState = std::make_shared<StateMenuShipSelect>();
            newState->setPlayerConfig(m_menuBase->m_playerConfig);
            getGame()->switchState(newState);
        },
        *this);
}
void StateMenuStageSelect::doInternalUpdate(float const elapsed)
{
    m_menuBase->update(elapsed);
    m_buttonBack->update(elapsed);

    for (auto& button : m_buttons) {
        button->update(elapsed);
    }
}
void StateMenuStageSelect::doInternalDraw() const
{
    m_menuBase->draw(getGame()->getRenderTarget());

    for (auto& button : m_buttons) {
        button->draw();
    }

    m_buttonBack->draw();
    m_menuBase->drawOverlay(getGame()->getRenderTarget());
}
void StateMenuStageSelect::createLevelButtons()
{
    nlohmann::json j;
    std::ifstream file { "assets/levels/__all_levels.json" };
    file >> j;

    auto levels = j["levels"];
    int counter = 0;

    for (auto l : levels) {
        std::string filename = l["filename"];
        std::string displayName = l["display"];

        float xPos = 41.0f;
        float yPos = 200.0f + 24.0f * (int)(counter / 3);
        unsigned int buttonWidth = 100U;

        xPos +=  (buttonWidth + 8.0f) * (counter % 3);

        auto button = std::make_shared<jt::Button>(jt::Vector2u { buttonWidth, 18 });
        add(button);
        if (m_menuBase->m_playerConfig.availableLevels.find(counter)
            == m_menuBase->m_playerConfig.availableLevels.end()) {
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

void StateMenuStageSelect::startTransitionToStateGame(std::string const& levelFilename)
{

    if (!m_started) {
        m_started = true;
        m_menuBase->m_playerConfig.selectedLevelFilename = levelFilename;
        createTweenTransition();
    }
}

void StateMenuStageSelect::setPlayerConfig(PlayerConfig const& pc)
{
    m_menuBase->m_playerConfig = pc;
}
