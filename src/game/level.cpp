#include "level.hpp"
#include "json.hpp"
#include <fstream>

Level::Level(std::string const filename)
{
    std::ifstream file { filename };

    nlohmann::json j;
    file >> j;

    parsePlayer(j);
    parseTransforms(j);
    parseEnemies(j);
    parseTargets(j);

    m_backgroundFilePath = "assets/backgrounds/" + j.value("background", "background_00.png");
}

void Level::parseEnemies(nlohmann::json const& j)
{
    auto enemiesIt = j.find("enemies");
    if (enemiesIt == j.end()) {
        return;
    }
    for (auto e : *enemiesIt) {
        EnemyLoadInfo info;
        auto t = std::make_shared<Transform>();
        info.transform = t;
        t->position = jt::Vector2 { e["x"].get<float>(), e["y"].get<float>() };

        // optional values
        t->velocity = jt::Vector2 { e.value("vx", 0.0f), e.value("vy", 0.0f) };
        info.flightAi = e.value("flight", "idle");
        info.shootAi = e.value("shoot", "idle");

        m_enemies.push_back(info);
    }
}
void Level::parseTransforms(nlohmann::json const& j)
{
    auto transformsIt = j.find("transforms");
    if (transformsIt == j.end()) {
        return;
    }
    for (auto p : *transformsIt) {
        auto t = std::make_shared<Transform>();
        t->position = jt::Vector2 { p["x"].get<float>(), p["y"].get<float>() };

        // optional values
        t->velocity = jt::Vector2 { p.value("vx", 0.0f), p.value("vy", 0.0f) };
        t->mass = p.value("mass", 1.0f);

        t->is_fixed = p.value("is_fixed", false);
        m_transforms.emplace_back(std::move(t));
    }
}
void Level::parsePlayer(nlohmann::json const& j)
{
    m_player_transform = std::make_shared<Transform>();
    auto p = j["player"];
    m_player_transform->position = jt::Vector2 { p["x"].get<float>(), p["y"].get<float>() };
    m_player_transform->velocity = jt::Vector2 { p["vx"].get<float>(), p["vy"].get<float>() };
    m_player_transform->angle = p.value("angle", 0.0f);
}

void Level::parseTargets(nlohmann::json const& j)
{
    auto targetsIt = j.find("targets");
    if (targetsIt == j.end()) {
        return;
    }
    for (auto t : *targetsIt) {
        m_targets.push_back(jt::Vector2 { t["x"], t["y"] });
    }
}

std::vector<std::shared_ptr<Transform>> Level::getPlanets() { return m_transforms; }
std::shared_ptr<Transform> Level::getPlayer() { return m_player_transform; }
std::vector<EnemyLoadInfo> Level::getEnemies() { return m_enemies; }
std::vector<jt::Vector2> Level::getTargets() { return m_targets; }
std::string Level::getBackgroundFilePath() { return m_backgroundFilePath; }
