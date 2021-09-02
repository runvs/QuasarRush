#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "enemy.hpp"
#include "enemy_load_info.hpp"
#include "explosion_spawn_interface.hpp"
#include "game_state.hpp"
#include "info_text.hpp"
#include "object_group.hpp"
#include "physics_system.hpp"
#include "planet.hpp"
#include "player.hpp"
#include "player_config.hpp"
#include "shot_mg.hpp"
#include "shot_spawn_interface.hpp"
#include "spawn_trail_interface.hpp"
#include "target.hpp"
#include "particle_system.hpp"
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
} // namespace jt

class Hud;

class StateGame : public jt::GameState, public ShotSpawnInterface, public ExplosionSpawnInterface, public SpawnTrailInterface {
public:
    void setLevel(std::string const& level_filename);
    void setPlayerConfig(PlayerConfig const& pc);
    void spawnShotMg(jt::Vector2 const& pos, jt::Vector2 const& dir, bool byPlayer) override;
    void spawnShotMissile(jt::Vector2 const& pos, jt::Vector2 const& dir, bool byPlayer) override;
    void spawnExplosion(jt::Vector2 const& position) override;
    void spawnTrail(jt::Vector2 pos) override;

private:
    std::shared_ptr<jt::Sprite> m_background;
    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;
    std::shared_ptr<Hud> m_hud;
    std::vector<std::shared_ptr<InfoText>> m_infoTexts;

    bool m_running { false };
    bool m_hasEnded { false };

    std::shared_ptr<Player> m_player;
    PlayerConfig m_playerConfig;

    jt::ObjectGroup<Planet> m_planets;
    std::unique_ptr<PhysicsSystem> m_physics_system;

    jt::ObjectGroup<ShotInterface> m_shots;

    jt::ObjectGroup<Enemy> m_enemies;

    jt::ObjectGroup<Target> m_targets;

    std::string m_level_filename;

    jt::Vector2 m_trailSpawnPosition;
    std::shared_ptr<jt::ParticleSystem<jt::Shape, 100>> m_trailParticles;

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void endGame();

    void createLevelEntities();
    void handleShotCollisions();
    void handlePlayerPlanetCollision();
    void handlePlayerTargetCollisions();
    void checkGameOver();
    void createTutorial();
    void createTutorialForFirstMission();
    void CreateTutorialForSecondMission();

    void CreateInfoText(std::shared_ptr<jt::DrawableInterface> target, std::string text,
        float timeOffset, eInfoTextAlign align);
    std::shared_ptr<EnemyAI> createFlightAi(EnemyLoadInfo& e) const;
    std::shared_ptr<EnemyAI> createShootAi(EnemyLoadInfo e);
};

#endif
