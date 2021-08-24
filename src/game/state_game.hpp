#ifndef GAME_STATE_GAME_HPP_INCLUDEGUARD
#define GAME_STATE_GAME_HPP_INCLUDEGUARD

#include "game_state.hpp"
#include "physics_system.hpp"
#include "planet.hpp"
#include "player.hpp"
#include <memory>
#include <vector>

// fwd decls
namespace jt {
class Shape;
class Sprite;
} // namespace jt

class Hud;

class StateGame : public jt::GameState {

private:
    std::shared_ptr<jt::Shape> m_background;
    std::shared_ptr<jt::Shape> m_overlay;
    std::shared_ptr<jt::Sprite> m_vignette;
    std::shared_ptr<Hud> m_hud;

    bool m_running { false };
    bool m_hasEnded { false };

    int m_scoreP1 { 0 };
    int m_scoreP2 { 0 };

    std::shared_ptr<Player> m_player;

    std::vector<std::shared_ptr<Planet>> m_planets;
    std::unique_ptr<PhysicsSystem> m_physics_system;

    void doInternalCreate() override;
    void doInternalUpdate(float const elapsed) override;
    void doInternalDraw() const override;

    void endGame();
};

#endif
