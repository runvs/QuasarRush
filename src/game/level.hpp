#ifndef QUASARRUSH_LEVEL_HPP
#define QUASARRUSH_LEVEL_HPP

#include "transform.hpp"
#include "enemy_load_info.hpp"
#include <json.hpp>
#include <memory>
#include <string>
#include <vector>

class Level {
public:
    explicit Level(std::string const filename);
    std::vector<std::shared_ptr<Transform>> getPlanets();
    std::shared_ptr<Transform> getPlayer();
    std::vector<EnemyLoadInfo> getEnemies();
    std::vector<jt::Vector2> getTargets();
    std::string getBackgroundFilePath();
    float getLevelTime() const;

private:
    std::shared_ptr<Transform> m_player_transform;
    std::vector<std::shared_ptr<Transform>> m_transforms;
    std::vector<EnemyLoadInfo> m_enemies;
    std::vector<jt::Vector2> m_targets;
    std::string m_backgroundFilePath;
    float m_levelTime;

    void parsePlayer(nlohmann::basic_json<> const& j);
    void parseTransforms(nlohmann::json const& j);
    void parseEnemies(nlohmann::json const& j);
    void parseTargets(nlohmann::json const& j);
    void parseBackgroundImage(nlohmann::json const& j);
};

#endif // QUASARRUSH_LEVEL_HPP
