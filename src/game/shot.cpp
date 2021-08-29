#include "shot.hpp"
#include "drawable_helpers.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

void Shot::doCreate()
{
    m_transform = std::make_shared<Transform>();
    m_sprite = std::make_shared<jt::Sprite>();
    m_sprite->loadSprite("assets/shot.png");
}

void Shot::doUpdate(float const elapsed)
{
    m_sprite->setPosition(m_transform->position);
    m_transform->angle = jt::MathHelper::angleOf(m_transform->velocity);
    m_sprite->setRotation(-m_transform->angle);
    m_sprite->update(elapsed);

    if (m_transform->position.x() < 0 || m_transform->position.x() > GP::GetScreenSize().x()) {
        kill();
    }
    if (m_transform->position.y() < 0 || m_transform->position.y() > GP::GetScreenSize().y()) {
        kill();
    }
    if (getAge() > 60.0f) {
        kill();
    }
}
void Shot::doDraw() const { m_sprite->draw(getGame()->getRenderTarget()); }
void Shot::doKill() { }

std::shared_ptr<Transform> Shot::getTransform() { return m_transform; }

void Shot::setTransform(std::shared_ptr<Transform> t) { m_transform = t; }
