#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "game_state.hpp"
#include "physics_system.hpp"
#include "planet.hpp"
#include "player.hpp"
#include "shot.hpp"
#include "enemy.hpp"
#include "object_group.hpp"
#include <memory>

#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {
public:
    void setLevel(std::string const& level_filename);
private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;
    std::shared_ptr<Hud> m_hud;

    bool m_running { false };
    bool m_hasEnded { false };

    std::shared_ptr<Player> m_player;

    jt::ObjectGroup<Planet> m_planets;
    std::unique_ptr<PhysicsSystem> m_physics_system;

    jt::ObjectGroup<Shot> m_shots;
    jt::ObjectGroup<Enemy> m_enemies;

    std::string m_level_filename;

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void endGame();
    void spawnShot();
    void createLevelEntities();
    void handleShotCollisions();
    void handlePlayerPlanetCollision();
};

#endif
