#ifndef QUASARRUSH_ENEMY_SHOOT_AI_MG_HPP
#define QUASARRUSH_ENEMY_SHOOT_AI_MG_HPP

#include "enemy_ai.hpp"
#include "shot_spawn_interface.hpp"
#include <memory>

class Player;

class EnemyShootAiMg :public EnemyAI{
public:
    EnemyShootAiMg(ShotSpawnInterface& shotSpawnInterface, std::shared_ptr<Player> player);
    void act(Enemy& enemy, float elapsed) override;
private:
    ShotSpawnInterface& m_shotSpawnInterface;
    std::weak_ptr<Player> m_player;
};

#endif // QUASARRUSH_ENEMY_SHOOT_AI_MG_HPP
