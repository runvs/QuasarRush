#include "physics_system.hpp"
#include "math_helper.hpp"

void PhysicsSystem::registerTransform(std::shared_ptr<Transform> transform)
{
    if (transform) {
        m_transforms.push_back(transform);
    }
}

void PhysicsSystem::reset_accelerations()
{
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
        auto const p2 = t2->position;

        auto dist = p2 - p1;

        float r = jt::MathHelper::length(dist);
        if (r < 0.00001)
        {
            continue;
        }
        jt::MathHelper::normalizeMe(dist, 5);
        auto force = dist / r * 10.0f * t1->mass * t2->mass;

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
    calculate_forces();

    integratePositions(elapsed);
}

std::shared_ptr<Transform> createDeepTransformCopy(std::shared_ptr<Transform> const& transform)
{
    auto tmp = std::make_shared<Transform>();
    tmp->acceleration = transform->acceleration;
    tmp->velocity = transform->velocity;
    tmp->position = transform->position;
    tmp->mass = transform->mass;
    tmp->is_fixed = transform->is_fixed;
    return tmp;
}

std::vector<jt::Vector2> PhysicsSystem::precalculate_path(std::shared_ptr<Transform> transform)
{
    std::vector<jt::Vector2> updated_positions {};

    auto tmp = createDeepTransformCopy(transform);

    int N = 80;
    int substeps = 10;
    for (int i = 0; i != N; ++i)
    {
        tmp->acceleration = jt::Vector2{0.0f, 0.0f};
        calculateForcesForSingleTransform(tmp);
        integrateSinglePosition(0.016f, tmp);
        substeps--;
        if (substeps == 0) {substeps= 10;

            updated_positions.push_back(tmp->position);
        }
    }

    return updated_positions;
}
