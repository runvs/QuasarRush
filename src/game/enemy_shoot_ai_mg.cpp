#include "enemy_shoot_ai_mg.hpp"
#include <iostream>
#include "player.hpp"
#include "state_game.hpp"

EnemyShootAiMg::EnemyShootAiMg(jt::GameState& gamestate, std::shared_ptr<Player> player)
    : m_gameState { gamestate }
    , m_player { player }
{
}

void EnemyShootAiMg::act(Enemy& enemy, float elapsed) {
    if (enemy.canShoot())
    {
        enemy.shoot();
        auto const playerPosition = m_player.lock()->getTransform()->position;
        auto const enemyPosition = enemy.getTransform()->position;



    }

}
