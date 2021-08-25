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
}

void Level::parseEnemies(nlohmann::json const& j)
{
    auto enemiesIt = j.find("enemies");
    if (enemiesIt == j.end())
    {
        return;
    }
    for (auto e : *enemiesIt)
    {
        auto t = std::make_shared<Transform>();
        t->position = jt::Vector2 { e["x"].get<float>(), e["y"].get<float>() };

        // optional values
        t->velocity = jt::Vector2 { e.value("vx", 0.0f), e.value("vy", 0.0f) };
        m_enemies.push_back(t);
    }
}
void Level::parseTransforms(nlohmann::json const& j)
{
    auto transforms = j["transforms"];
    for (auto p : transforms) {
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
}

std::vector<std::shared_ptr<Transform>> Level::getTransforms() { return m_transforms; }
std::shared_ptr<Transform> Level::getPlayer() { return m_player_transform; }
std::vector<std::shared_ptr<Transform>> Level::getEnemies()
{
    return m_enemies;
}
