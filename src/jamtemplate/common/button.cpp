#include "button.hpp"
#include "animation.hpp"
#include "drawable_impl.hpp"
#include "drawable_interface.hpp"
#include "game_interface.hpp"
#include "game_object.hpp"
#include "input_manager.hpp"
#include "sprite.hpp"
#include "vector.hpp"
#include <functional>
#include <memory>
#include <string>

namespace jt {

Button::Button(jt::Vector2u s)
{
    std::string buttonImageName = "#b#" + std::to_string(s.x()) + "#" + std::to_string(s.y());
    m_background = std::make_shared<jt::Animation>();
    m_background->add(buttonImageName, "normal", s, { 0 }, 1);
    m_background->add(buttonImageName, "over", s, { 1 }, 1);
    m_background->add(buttonImageName, "down", s, { 2 }, 1);
    m_background->play("normal");
    m_background->setIgnoreCamMovement(true);

    m_disabledOverlay = std::make_shared<jt::Sprite>();
    m_disabledOverlay->loadSprite("#f#" + std::to_string(s.x()) + "#" + std::to_string(s.y()));
    m_disabledOverlay->setColor(jt::Color { 100, 100, 100, 150 });

    setSound("assets/sfx/bump1.ogg");
}

Button::~Button()
{
    m_drawable = nullptr;
    m_background = nullptr;
    m_callbacks.clear();
}

void Button::setDrawable(std::shared_ptr<DrawableInterface> sprt) { m_drawable = sprt; }

void Button::addCallback(std::function<void(void)> cb) { m_callbacks.push_back(cb); }

void Button::clearCallbacks() { m_callbacks.clear(); }
size_t Button::getCallbackCount() const { return m_callbacks.size(); }

bool Button::IsMouseOver()
{
    if (getGame()->input() == nullptr) {
        return false;
    }
    return isOver(getGame()->input()->mouse()->getMousePositionScreen());
}

void Button::setVisible(bool v) { m_isVisible = v; }

bool Button::getVisible() const { return m_isVisible; }

void Button::setPosition(jt::Vector2 const& v) { m_pos = v; }

jt::Vector2 Button::getPosition() const { return m_pos; }

void Button::doDraw() const
{
    if (!m_isVisible) {
        return;
    }

    m_background->draw(getGame()->getRenderTarget());
    if (m_drawable)
        m_drawable->draw(getGame()->getRenderTarget());

    if (!m_isActive) {
        m_disabledOverlay->draw(getGame()->getRenderTarget());
    }
}

bool Button::isOver(jt::Vector2 const& mousePosition)
{
    if (!m_isActive) {
        return false;
    }

    float px = m_background->getPosition().x();
    float py = m_background->getPosition().y();

    float w = m_background->getGlobalBounds().width();
    float h = m_background->getGlobalBounds().height();
    return (mousePosition.x() > px && mousePosition.x() <= px + w && mousePosition.y() > py
        && mousePosition.y() <= py + h);
}

void Button::doUpdate(float elapsed)
{
    m_background->update(elapsed);
    m_disabledOverlay->update(elapsed);

    m_disabledOverlay->setPosition((m_pos));
    m_background->setPosition(m_pos);

    if (m_drawable) {
        m_drawable->setPosition(m_pos);
        m_drawable->update(elapsed);
    }

    if (IsMouseOver()) {
        if (getGame()->input()->mouse()->pressed(jt::MouseButtonCode::MBLeft)) {
            m_background->play("down");
        } else {
            m_background->play("over");
        }

        if (getGame()->input()->mouse()->justReleased(jt::MouseButtonCode::MBLeft)) {
            if (m_isVisible) {
                m_sound->play();
                for (auto& cb : m_callbacks) {

                    cb();
                }
            }
        }
    } else {
        m_background->play("normal");
    }
}
bool Button::getActive() const { return m_isActive; }
void Button::setActive(bool v) { m_isActive = v; }
std::shared_ptr<Animation> Button::getAnimation() const { return m_background; }

void Button::setSound(std::string path)
{
    m_sound = std::make_unique<jt::SoundGroup>(std::vector<std::string> { "assets/sfx/bump1.ogg" });
}

} // namespace jt
