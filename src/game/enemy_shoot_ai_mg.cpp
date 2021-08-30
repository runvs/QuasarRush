#include "enemy_shoot_ai_mg.hpp"
#include "player.hpp"
#include "state_game.hpp"
#include <math_helper.hpp>
#include "state_game.hpp"

EnemyShootAiMg::EnemyShootAiMg(ShotSpawnInterface& shotSpawnInterface, std::shared_ptr<Player> player)
    : m_shotSpawnInterface { shotSpawnInterface }
    , m_player { player }
{
}

void EnemyShootAiMg::act(Enemy& enemy, float elapsed) {
    if (enemy.canShoot())
    {
        enemy.shoot();
        auto const playerPosition = m_player.lock()->getTransform()->position;
        auto const enemyPosition = enemy.getTransform()->position;

        auto dir = playerPosition - enemyPosition;
        jt::MathHelper::normalizeMe(dir);

        m_shotSpawnInterface.spawnShot(enemyPosition + 4.0f * dir, dir, false);
    }
}
