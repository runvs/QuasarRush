#ifndef QUASARRUSH_ENEMY_FLIGHT_AI_STAY_HPP
#define QUASARRUSH_ENEMY_FLIGHT_AI_STAY_HPP

#include "enemy_ai.hpp"
#include "vector.hpp"

class EnemyFlightAiStay : public EnemyAI{
public:
    EnemyFlightAiStay(jt::Vector2 pos);
    void act(Enemy& enemy, float elapsed) override;
private:
    jt::Vector2 m_position;
};

#endif // QUASARRUSH_ENEMY_FLIGHT_AI_STAY_HPP
