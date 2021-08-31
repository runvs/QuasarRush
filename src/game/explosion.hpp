#ifndef EXPLOSION_GUARD_HPP
#define EXPLOSION_GUARD_HPP

#include "animation.hpp"
#include "game_object.hpp"
#include <memory>

class Explosion : public jt::GameObject {
private:
    std::shared_ptr<jt::Animation> m_sprite;
    float const m_frameTime { 0.10f };
    int const m_numberOfFrames { 8 };

public:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    std::shared_ptr<jt::Animation> getAnimation() const;
};

#endif // EXPLOSION_GUARD_HPP
