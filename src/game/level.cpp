#include "level.hpp"
#include "json.hpp"
#include <fstream>

Level::Level(std::string const filename)
{
    std::ifstream file { filename };

    nlohmann::json j;
    file >> j;
    auto transforms = j["transforms"];
    for (auto p : transforms)
    {
        auto t = std::make_shared<Transform>();
        t->position = jt::Vector2{p["x"].get<float>(), p["y"].get<float>()};
        t->velocity = jt::Vector2{p["vx"].get<float>(), p["vy"].get<float>()};
        m_transforms.emplace_back(std::move(t));
    }
}

std::vector<std::shared_ptr<Transform>> Level::getTransforms()
{
    return m_transforms;
}
