#include "state_game.hpp"
#include "color.hpp"
#include "enemy_ai_idle.hpp"
#include "enemy_flight_ai_stay.hpp"
#include "enemy_shoot_ai_mg.hpp"
#include "explosion.hpp"
#include "game_interface.hpp"
#include "game_properties.hpp"
#include "hud/hud.hpp"
#include "level.hpp"
#include "line.hpp"
#include "math_helper.hpp"
#include "shape.hpp"
#include "shot_missile.hpp"
#include "sprite.hpp"
#include "state_menu.hpp"
#include "tween_alpha.hpp"
#include "tween_position.hpp"
#include "tween_scale.hpp"
#include <algorithm>

void StateGame::doInternalCreate()
{
    float const w = static_cast<float>(GP::GetWindowSize().x());
    float const h = static_cast<float>(GP::GetWindowSize().y());

    using jt::Shape;
    using jt::TweenAlpha;

    m_background = std::make_shared<jt::Sprite>();
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

    m_trailParticles = jt::ParticleSystem<jt::Shape, 100>::create(
        []() {
            auto s = std::make_shared<jt::Shape>();
            s->makeRect(jt::Vector2 { 2.0f, 2.0f });
            s->setPosition(jt::Vector2 { -100.0f, -100.0f });
            return s;
        },
        [this](auto s) {
            s->setPosition(m_trailSpawnPosition);

            auto tw = jt::TweenAlpha<jt::Shape>::create(s, 0.8f, 100, 0);
            add(tw);
            float spread = 6;
            auto offset
                = jt::Random::getRandomPointin({ -spread, -spread, spread * 2.0f, spread * 2.0f });
            auto tp = jt::TweenPosition<jt::Shape>::create(
                s, 0.8f, m_trailSpawnPosition, m_trailSpawnPosition + offset);
            add(tp);

            auto ts = jt::TweenScale<jt::Shape>::create(s,0.4f,{1.0f, 1.0f}, {1.5f, 1.5f});
            ts->setStartDelay(0.4f);
            add(ts);
        });
    add(m_trailParticles);

    createLevelEntities();

    m_hud = std::make_shared<Hud>();
    add(m_hud);

    // StateGame will call drawObjects itself.
    setAutoDraw(false);

    createTutorial();
}
void StateGame::createTutorial()
{
    if (m_level_filename == "1_empty_space_and_targets.json") {
        createTutorialForFirstMission();
    } else if (m_level_filename == "2_planet_and_targets.json") {
        CreateTutorialForSecondMission();
    } else if (m_level_filename == "3_planet_and_enemy.json") {
        CreateInfoText(m_enemies.back().lock()->getSprite(), "Enemy !", 0.0f, RightUp);
        CreateInfoText(m_player->getSprite(), "Aim and Shoot with mouse", 4.0f, LeftDown);
    }
}
void StateGame::CreateTutorialForSecondMission()
{
    CreateInfoText(m_planets.front().lock()->getSprite(), "Gravity Well", 0.0f, RightDown);

    CreateInfoText(m_targets.back().lock()->getShape(), "Mission End", 7.0f, LeftUp);
    CreateInfoText(m_player->getSprite(), "Those two were easy", 11.0f, RightDown);
}

void StateGame::CreateInfoText(std::shared_ptr<jt::DrawableInterface> target, std::string text,
    float timeOffset, eInfoTextAlign align)
{
    auto info = std::make_shared<InfoText>(target, text, align);
    add(info);

    auto twaInt = jt::TweenAlpha<InfoText>::create(info, 0.75f, 0, 255);
    twaInt->setStartDelay(timeOffset + 0.75f);
    add(twaInt);

    auto twaOut = jt::TweenAlpha<InfoText>::create(info, 0.75f, 255, 0);
    twaOut->addCompleteCallback([info]() { info->kill(); });
    twaOut->setStartDelay(timeOffset + 3.25f);
    add(twaOut);
}

void StateGame::createTutorialForFirstMission()
{
    CreateInfoText(m_player->getSprite(), "Your Ship", 0.0f, RightDown);
    CreateInfoText(m_targets.front().lock()->getShape(), "Nav Target", 4.0f, RightUp);
    CreateInfoText(m_player->getSprite(), "Accelerate with 'W'", 8.0f, RightUp);
    CreateInfoText(m_player->getSprite(), "Rotate with 'A'/'D'", 12.0f, RightUp);
    CreateInfoText(m_targets.at(1).lock()->getShape(), "Correct Course", 16.0f, LeftDown);
    CreateInfoText(m_targets.back().lock()->getShape(), "Mission End", 20.0f, LeftUp);
}

void StateGame::createLevelEntities()
{
    Level l("assets/levels/" + m_level_filename);
    m_player = std::make_shared<Player>(*this, *this, m_playerConfig);
    add(m_player);
    m_player->setTransform(l.getPlayer());

    m_physics_system->registerTransform(m_player->getTransform());

    for (auto t : l.getPlanets()) {
        auto object = std::make_shared<Planet>();
        add(object);
        object->setTransform(t);
        m_physics_system->registerTransform(t);
        m_planets.push_back(object);
    }

    for (auto e : l.getEnemies()) {
        auto enemy = std::make_shared<Enemy>(*this);
        add(enemy);
        enemy->setTransform(e.transform);

        enemy->setFlightAi(createFlightAi(e));
        enemy->setShootAi(createShootAi(e));

        m_physics_system->registerTransform(enemy->getTransform());
        m_enemies.push_back(enemy);
    }
    for (auto p : l.getTargets()) {
        auto t = std::make_shared<Target>(p);
        add(t);
        m_targets.push_back(t);
    }

    m_background->loadSprite(l.getBackgroundFilePath());
    m_background->setScale(jt::Vector2 { 0.5f, 0.5f });
}
std::shared_ptr<EnemyAI> StateGame::createShootAi(EnemyLoadInfo e)
{
    std::shared_ptr<EnemyAI> shootAi;
    if (e.shootAi == "idle") {
        shootAi = std::make_shared<EnemyAIIdle>();
    } else if (e.shootAi == "shootMG") {
        shootAi = std::make_shared<EnemyShootAiMg>(*this, m_player);
    } else {
        std::cout << "invalid shoot ai type: " << e.shootAi << std::endl;
        throw std::invalid_argument { "invalid shoot ai type: " + e.shootAi };
    }

    return shootAi;
}

std::shared_ptr<EnemyAI> StateGame::createFlightAi(EnemyLoadInfo& e) const
{
    std::shared_ptr<EnemyAI> flightAi;
    if (e.flightAi == "stay") {
        flightAi = std::make_shared<EnemyFlightAiStay>(e.transform->position);
    } else if (e.flightAi == "idle") {
        flightAi = std::make_shared<EnemyAIIdle>();
    } else {
        std::cout << "invalid flight ai type: " << e.flightAi << std::endl;
        throw std::invalid_argument { "invalid flight ai type: " + e.flightAi };
    }
    return flightAi;
}

void StateGame::doInternalUpdate(float const elapsed)
{
    if (m_running) {

        m_player->setProjectionPoints(m_physics_system->precalculate_path(
            m_player->getTransform(), m_playerConfig.sensorLevel));

        handleShotCollisions();
        handlePlayerPlanetCollision();
        handlePlayerTargetCollisions();

        checkGameOver();

        m_physics_system->update(elapsed);
        m_physics_system->update(elapsed);

        m_enemies.update(elapsed);
        m_targets.update(elapsed);
    }

    m_background->update(elapsed);
    m_vignette->update(elapsed);
    m_overlay->update(elapsed);
}

void StateGame::handlePlayerTargetCollisions()
{
    auto const playerPosition = m_player->getTransform()->position;

    for (auto tptr : m_targets) {
        if (tptr.expired()) {
            continue;
        }
        auto target = tptr.lock();
        if (target->hasBeenHit()) {
            continue;
        }
        auto const targetPosition = target->getPosition();
        auto diff = targetPosition - playerPosition;
        auto lengthSquared = jt::MathHelper::lengthSquared(diff);

        if (lengthSquared <= GP::PlayerHalfSize() * GP::PlayerHalfSize()) {
            for (auto tw : target->hit()) {
                add(tw);
            }
        }
    }
}
void StateGame::handlePlayerPlanetCollision()
{
    auto const playerPos = m_player->getTransform()->position;
    for (auto pptr : m_planets) {
        if (pptr.expired()) {
            continue;
        }
        auto p = pptr.lock();
        auto const planetPos = p->getTransform()->position - jt::Vector2 { 4.0f, 0.0f };

        auto const diff = planetPos - playerPos;
        auto const lsquared = jt::MathHelper::lengthSquared(diff);

        if (lsquared <= (GP::PlayerHalfSize() + GP::PlanetHalfSize())
                * (GP::PlayerHalfSize() + GP::PlanetHalfSize())) {
            endGame();
        }
    }
}
void StateGame::handleShotCollisions()
{
    for (auto sptr : m_shots) {
        if (sptr.expired()) {
            continue;
        }

        auto s = sptr.lock();
        auto const sp = s->getTransform()->position;

        if (!s->getFiredByPlayer()) {
            auto const pp = m_player->getTransform()->position;
            auto const diff = pp - sp;
            auto const lengthSquared = jt::MathHelper::lengthSquared(diff);
            if (lengthSquared <= GP::EnemyHalfSize() * GP::EnemyHalfSize()) {
                s->hit();
            }
        } else {
            for (auto eptr : m_enemies) {
                if (eptr.expired()) {
                    continue;
                }
                auto e = eptr.lock();
                auto const ep = e->getTransform()->position;
                auto const diff = ep - sp;
                auto const lengthSquared = jt::MathHelper::lengthSquared(diff);

                if (lengthSquared <= GP::EnemyHalfSize() * GP::EnemyHalfSize()) {
                    s->hit();
                    e->takeDamage();
                }
            }
        }

        for (auto pptr : m_planets) {
            if (pptr.expired()) {
                continue;
            }
            auto p = pptr.lock();
            jt::Vector2 const& planteOffset = jt::Vector2 { -0.5f, -0.5f };
            auto const ep = p->getTransform()->position + planteOffset;
            auto const diff = ep - sp;
            auto const lengthSquared = jt::MathHelper::lengthSquared(diff);
            if (lengthSquared <= GP::PlanetHalfSize() * GP::PlanetHalfSize()) {
                s->hit();
            }
        }
    }
}

void StateGame::spawnShotMg(jt::Vector2 const& pos, jt::Vector2 const& dir, bool byPlayer)
{
    auto shot = std::make_shared<ShotMg>();
    add(shot);
    shot->setFiredByPlayer(byPlayer);
    auto transform = shot->getTransform();
    transform->velocity = dir * GP::ShotSpeed();
    transform->position = pos + dir * 7.0f;
    transform->mass = 0.000001f;
    transform->is_force_emitter = false;
    m_shots.push_back(shot);
    m_physics_system->registerTransform(transform);
}

void StateGame::spawnExplosion(jt::Vector2 const& position)
{
    auto explosion = std::make_shared<Explosion>();
    add(explosion);
    explosion->getAnimation()->setPosition(position);
}

void StateGame::spawnShotMissile(jt::Vector2 const& pos, jt::Vector2 const& dir, bool byPlayer)
{

    for (int i = 0; i != 5; ++i) {
        auto shot = std::make_shared<ShotMissile>(*this);
        add(shot);
        shot->setFiredByPlayer(byPlayer);
        if (m_enemies.empty()) {
            // TODO
            return;
        }

        shot->setTarget(m_enemies.at(0).lock()->getTransform());
        auto transform = shot->getTransform();

        transform->velocity = jt::MathHelper::rotateBy(dir, jt::Random::getFloatGauss(0.0f, 35.0f))
            * GP::ShotSpeed() * 1.25f;
        transform->position = pos + dir * 7.0f;
        transform->mass = 0.1f;
        transform->is_force_emitter = false;
        m_shots.push_back(shot);
        m_physics_system->registerTransform(transform);
    }
}
void StateGame::doInternalDraw() const
{
    m_background->draw(getGame()->getRenderTarget());
    //    m_trailParticles->draw();
    drawObjects();
    m_vignette->draw(getGame()->getRenderTarget());
    m_hud->draw();

    for (auto& t : m_infoTexts) {
        t->draw();
    }

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
    tw->addCompleteCallback([this]() {
        auto newState = std::make_shared<StateMenu>();

        int maxCurrentLevel = *std::max_element(
            m_playerConfig.availableLevels.begin(), m_playerConfig.availableLevels.end());
        m_playerConfig.availableLevels.insert(maxCurrentLevel + 1);

        newState->setPlayerConfig(m_playerConfig); // keep playerConfig consistent

        getGame()->switchState(newState);
    });
    add(tw);
}

void StateGame::setLevel(std::string const& level_filename) { m_level_filename = level_filename; }

void StateGame::checkGameOver()
{
    if (m_targets.empty() && m_enemies.empty()) {
        endGame();
    }
}
void StateGame::setPlayerConfig(PlayerConfig const& pc) { m_playerConfig = pc; }
void StateGame::spawnTrail(jt::Vector2 pos)
{
    static int counter { 0 };
    counter++;
    if (counter % 3 == 0) {
        m_trailSpawnPosition = pos;
        m_trailParticles->Fire();
    }
}
