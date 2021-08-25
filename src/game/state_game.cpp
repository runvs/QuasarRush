#include "state_game.hpp"
#include "color.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "hud/hud.hpp"
#include "level.hpp"
#include "math_helper.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "state_menu.hpp"
#include "tween_alpha.hpp"

void StateGame::doInternalCreate()
{
    float const w = static_cast<float>(GP::GetWindowSize().x());
    float const h = static_cast<float>(GP::GetWindowSize().y());

    using jt::Shape;
    using jt::TweenAlpha;

    m_background = std::make_shared<Shape>();
    m_background->makeRect({ w, h });
    m_background->setColor(GP::PaletteBackground());
    m_background->setIgnoreCamMovement(true);
    m_background->update(0.0f);

    m_overlay = std::make_shared<Shape>();
    m_overlay->makeRect(jt::Vector2 { w, h });
    m_overlay->setColor(jt::Color { 0, 0, 0 });
    m_overlay->setIgnoreCamMovement(true);
    m_overlay->update(0);
    auto tw = TweenAlpha<Shape>::create(m_overlay, 0.5f, std::uint8_t { 255 }, std::uint8_t { 0 });
    tw->setSkipFrames();
    tw->addCompleteCallback([this]() { m_running = true; });
    add(tw);

    m_vignette = std::make_shared<jt::Sprite>();
    m_vignette->loadSprite("#v#" + std::to_string(static_cast<int>(GP::GetScreenSize().x())) + "#"
        + std::to_string(static_cast<int>(GP::GetScreenSize().y())));
    m_vignette->setIgnoreCamMovement(true);
    m_vignette->setColor({ 255, 255, 255, 100 });

    m_physics_system = std::make_unique<PhysicsSystem>();

    Level l("assets/levels/"+ m_level_filename);
    m_player = std::make_shared<Player>();
    add(m_player);
    m_player->setTransform(l.getPlayer());

    m_physics_system->registerTransform(m_player->getTransform());

    for (auto t : l.getTransforms()) {
        auto object = std::make_shared<Planet>();
        add(object);
        object->setTransform(t);
        m_physics_system->registerTransform(t);
        m_planets.push_back(object);
    }

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {
        m_physics_system->reset_accelerations();
        m_player->update(elapsed);
        m_player->setProjectionPoints(
            m_physics_system->precalculate_path(m_player->getTransform()));

        for (auto o : m_planets) {
            o->update(elapsed);
        }

        if (getGame()->input()->mouse()->pressed(jt::MouseButtonCode::MBLeft)) {
            if (m_player->canShoot()) {
                m_player->shoot();

                spawnShot();
            }
        }

        m_physics_system->update(elapsed * 2.0f);
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
    m_overlay->update(elapsed);
}
void StateGame::spawnShot()
{
    auto shot = std::make_shared<Shot>();
    add(shot);
    auto const playerTransform = m_player->getTransform();
    auto transform = std::make_shared<Transform>();
    auto const mouse_position = getGame()->input()->mouse()->getMousePositionScreen();
    auto aim_direction = mouse_position - playerTransform->position;
    jt::MathHelper::normalizeMe(aim_direction);
    transform->velocity = aim_direction * 30.0f;

    transform->position
        = jt::Vector2 { playerTransform->position.x(), playerTransform->position.y() }
        + aim_direction * 5.0f;
    transform->mass = 0.000001f;
    transform->is_force_emitter = false;
    shot->setTransform(transform);

    m_physics_system->registerTransform(transform);
    m_shots.push_back(shot);
}

void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    drawObjects();
    m_vignette->draw(getGame()->getRenderTarget());
    m_hud->draw();
    m_overlay->draw(getGame()->getRenderTarget());
}

void StateGame::endGame()
{
    if (m_hasEnded) {
        // trigger this function only once
        return;
    }
    m_hasEnded = true;
    m_running = false;

    auto tw = jt::TweenAlpha<jt::Shape>::create(
        m_overlay, 0.5f, std::uint8_t { 0 }, std::uint8_t { 255 });
    tw->setSkipFrames();
    tw->addCompleteCallback([this]() { getGame()->switchState(std::make_shared<StateMenu>()); });
    add(tw);
}

void StateGame::setLevel(std::string const& level_filename) {
    m_level_filename = level_filename;
}
