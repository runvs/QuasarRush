#ifndef QUASARRUSH_LEVEL_HPP
#define QUASARRUSH_LEVEL_HPP

#include "transform.hpp"
#include <json.hpp>
#include <memory>
#include <string>
#include <vector>

class Level {
public:
    explicit Level(std::string const filename);
    std::vector<std::shared_ptr<Transform>> getTransforms();

    std::shared_ptr<Transform> getPlayer();

    std::vector<std::shared_ptr<Transform>> getEnemies();

private:

    std::shared_ptr<Transform> m_player_transform;

    std::vector<std::shared_ptr<Transform>> m_transforms;

    std::vector<std::shared_ptr<Transform>> m_enemies;
    void parsePlayer(nlohmann::basic_json<> const& j);
    void parseTransforms(nlohmann::json const& j);
    void parseEnemies(nlohmann::json const& j);
};

#endif // QUASARRUSH_LEVEL_HPP
