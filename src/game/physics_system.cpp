#include "physics_system.hpp"
#include "game_properties.hpp"
#include "math_helper.hpp"

void PhysicsSystem::registerTransform(std::shared_ptr<Transform> transform)
{
    if (transform) {
        m_transforms.push_back(transform);
    }
}

void PhysicsSystem::reset_accelerations()
{
    cleanUpTransforms();
    for (auto tptr : m_transforms) {
        auto t = tptr.lock();
        t->acceleration = jt::Vector2 { 0.0f, 0.0f };
    }
}

void PhysicsSystem::calculate_forces()
{
    for (auto tptr1 : m_transforms) {
        auto t1 = tptr1.lock();
        calculateForcesForSingleTransform(t1);
    }
}

void PhysicsSystem::calculateForcesForSingleTransform(std::shared_ptr<Transform> t1)
{
    auto const p1 = t1->position;
    for (auto tptr2 : m_transforms) {
        auto t2 = tptr2.lock();
        if (t1 == t2) {
            continue;
        }
        if (!t2->is_force_emitter) {
            continue;
        }
        auto const p2 = t2->position;

        auto dist = p2 - p1;

        float r = jt::MathHelper::length(dist);

        jt::MathHelper::normalizeMe(dist, 5);
        auto force = dist / r / r * 1000.0f * t1->mass * t2->mass;

        if (!t1->is_fixed) {
            t1->acceleration += force / t1->mass;
        }
    }
}

void PhysicsSystem::integratePositions(float elapsed)
{
    for (auto tptr : m_transforms) {
        auto t = tptr.lock();
        integrateSinglePosition(elapsed, t);
    }
}
void PhysicsSystem::integrateSinglePosition(float elapsed, std::shared_ptr<Transform> t) const
{
    if (!t->is_fixed) {

        t->velocity += t->acceleration * elapsed;
        t->position += t->velocity * elapsed;
    }
}

void PhysicsSystem::update(float elapsed)
{

    cleanUpTransforms();
    calculate_forces();
    integratePositions(elapsed / 2);
}

void PhysicsSystem::cleanUpTransforms()
{
    m_transforms.erase(std::remove_if(m_transforms.begin(), m_transforms.end(),
                           [](std::weak_ptr<Transform> wptr) { return wptr.expired(); }),
        m_transforms.end());
}

std::shared_ptr<Transform> createDeepTransformCopy(std::shared_ptr<Transform> const& transform)
{
    return std::make_shared<Transform>(*transform);
}

std::vector<jt::Vector2> PhysicsSystem::precalculate_path(std::shared_ptr<Transform> transform)
{
    std::vector<jt::Vector2> updated_positions {};

    auto backup = createDeepTransformCopy(transform);

    int N = 1000;
    int draw_resolution = 100;
    for (int i = 0; i != N; ++i) {
        transform->acceleration = jt::Vector2 { 0.0f, 0.0f };
        calculateForcesForSingleTransform(transform);
        integrateSinglePosition(0.02f, transform);
        if (i % draw_resolution == 0) {
            updated_positions.push_back(transform->position);
        }
    }

    // reset original transform values
    *transform = *backup;

    return updated_positions;
}
