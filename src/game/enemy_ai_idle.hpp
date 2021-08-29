#ifndef QUASARRUSH_ENEMY_AI_IDLE_HPP
#define QUASARRUSH_ENEMY_AI_IDLE_HPP

#include "enemy_ai.hpp"

class EnemyAIIdle : public EnemyAI {
public:
    void act(Enemy& enemy, float elapsed) override;
};

#endif // QUASARRUSH_ENEMY_AI_IDLE_HPP
