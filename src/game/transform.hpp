#ifndef GAME_TRANSFORM_GUARD_HPP
#define GAME_TRANSFORM_GUARD_HPP

#include "vector.hpp"

struct Transform {
    jt::Vector2 position { 0.0f, 0.0f };
    jt::Vector2 velocity { 0.0f, 0.0f };
    jt::Vector2 acceleration { 0.0f, 0.0f };

    /// in degree
    float angle { 0.0f };

    float mass { 1.0f };

    bool is_fixed{false};
    bool is_actor{true};
};

#endif
