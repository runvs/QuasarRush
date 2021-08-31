#ifndef QUASARRUSH_ENEMY_LOAD_INFO_HPP
#define QUASARRUSH_ENEMY_LOAD_INFO_HPP

#include "transform.hpp"
#include <memory>
#include <string>

struct EnemyLoadInfo
{
    std::shared_ptr<Transform> transform;
    std::string flightAi;
    std::string shootAi;
};

#endif // QUASARRUSH_ENEMY_LOAD_INFO_HPP
