#ifndef QUASARRUSH_LEVEL_HPP
#define QUASARRUSH_LEVEL_HPP

#include "transform.hpp"
#include <memory>
#include <string>
#include <vector>

class Level {
public:
    explicit Level(std::string const filename);
    std::vector<std::shared_ptr<Transform>> getTransforms();

    std::shared_ptr<Transform> getPlayer();

private:
    std::vector<std::shared_ptr<Transform>> m_transforms;

    std::shared_ptr<Transform> m_player_transform;
};

#endif // QUASARRUSH_LEVEL_HPP
