#ifndef QUASARRUSH_MISSILE_HPP
#define QUASARRUSH_MISSILE_HPP

#include "game_object.hpp"
#include "shot_interface.hpp"
#include "sprite.hpp"
#include <memory>

class ShotMissile : public jt::GameObject, public ShotInterface {
private:
    std::shared_ptr<jt::Sprite> m_sprite;
    bool m_firedByPlayer;
    std::shared_ptr<Transform> m_transform;
    std::weak_ptr<Transform> m_target;



public:
    std::shared_ptr<Transform> getTransform() override;
    bool getFiredByPlayer() const override;
    void hit() override;
    void setTarget(std::weak_ptr<Transform> target);

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;

    void setTransform(std::shared_ptr<Transform> t);
    void setFiredByPlayer(bool value);
};

#endif // QUASARRUSH_MISSILE_HPP
