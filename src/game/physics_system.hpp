#ifndef GAME_PHYSICS_SYSTEM_GUARD_HPP
#define GAME_PHYSICS_SYSTEM_GUARD_HPP

#include <vector>
#include <memory>
#include "transform.hpp"

class PhysicsSystem {
public:
    void registerTransform(std::shared_ptr<Transform> transform);
    void reset_accelerations();
    void calculate_forces();
    void integratePositions(float elapsed);

    void update(float elapsed);

    std::vector<jt::Vector2> precalculate_path(std::shared_ptr<Transform> transform);

private:
    std::vector<std::weak_ptr<Transform>> m_transforms;

    void calculateForcesForSingleTransform(std::shared_ptr<Transform> t1);
    void integrateSinglePosition(float elapsed, std::shared_ptr<Transform> t) const;
};

#endif 
