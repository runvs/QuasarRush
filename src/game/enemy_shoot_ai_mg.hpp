#ifndef QUASARRUSH_ENEMY_SHOOT_AI_MG_HPP
#define QUASARRUSH_ENEMY_SHOOT_AI_MG_HPP

#include "enemy_ai.hpp"
#include "game_state.hpp"

class Player;

class EnemyShootAiMg :public EnemyAI{
public:
    EnemyShootAiMg(jt::GameState& gamestate, std::shared_ptr<Player> player);
    void act(Enemy& enemy, float elapsed) override;
private:
    jt::GameState& m_gameState;
    std::weak_ptr<Player> m_player;
};

#endif // QUASARRUSH_ENEMY_SHOOT_AI_MG_HPP
