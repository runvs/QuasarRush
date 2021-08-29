#include "enemy_flight_ai_stay.hpp"
#include "enemy.hpp"

EnemyFlightAiStay::EnemyFlightAiStay(jt::Vector2 pos)
    : m_position { pos }
{
}

void EnemyFlightAiStay::act(Enemy& enemy, float elapsed)
{
    auto t = enemy.getTransform();
    t->position = m_position;
    enemy.setTransform(t);
}
