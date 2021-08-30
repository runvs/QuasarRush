#ifndef SHOT_GUARD_HPP
#define SHOT_GUARD_HPP

#include "game_object.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>

class Shot : public jt::GameObject {
private:
    std::shared_ptr<jt::Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;
    bool m_fired_by_player{true};

public:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    std::shared_ptr<Transform> getTransform();
    void setTransform(std::shared_ptr<Transform> t);
    void setFiredByPlayer(bool value);
    bool getFiredByPlayer() const;
};

#endif
