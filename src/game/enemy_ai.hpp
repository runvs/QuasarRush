#ifndef QUASARRUSH_ENEMY_AI_HPP
#define QUASARRUSH_ENEMY_AI_HPP

class Enemy;

class EnemyAI {
public:
    virtual void act(Enemy& enemy, float elapsed) = 0;
    virtual ~EnemyAI() = default;
};

#endif // QUASARRUSH_ENEMY_AI_HPP
