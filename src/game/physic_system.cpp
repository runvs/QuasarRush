#include "physic_system.hpp"
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
    for (auto i = 0U; i != m_transforms.size(); ++i) {
        auto t1 = m_transforms.at(i).lock();
        auto const p1 = t1->position;
        for (auto j = i + 1; j != m_transforms.size();
             ++j) { // start at i+1 so no self-interaction is happening
            auto t2 = m_transforms.at(j).lock();
            auto const p2 = t2->position;

            auto dist = p2 - p1;
            float r = jt::MathHelper::length(dist);
            jt::MathHelper::normalizeMe(dist, 5);
            auto force = dist / r * 10.0f * t1->mass * t2->mass;

            t1->acceleration += force / t1->mass;
            t2->acceleration -= force / t2->mass;
        }
    }
}

void PhysicsSystem::integrate_positions(float elapsed)
{
    for (auto tptr : m_transforms) {
        auto t = tptr.lock();
        t->velocity += t->acceleration * elapsed;
        t->position+= t->velocity* elapsed;

    }
}

void PhysicsSystem::update(float elapsed)
{
    reset_accelerations();

    calculate_forces();

    integrate_positions(elapsed);
}

std::vector<jt::Vector2> PhysicsSystem::precalculate_path(std::shared_ptr<Transform> transform)
{
    return {};
}
