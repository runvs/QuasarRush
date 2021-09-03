#ifndef IMPACT_EXPLOSION_HPP_GUARD
#define IMPACT_EXPLOSION_HPP_GUARD

#include "animation.hpp"
#include "game_object.hpp"
#include <memory>

class ImpactExplosion : public jt::GameObject {
private:
    std::shared_ptr<jt::Animation> m_sprite;
    float const m_frameTime { 0.10f };
    int const m_numberOfFrames { 4 };

public:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    std::shared_ptr<jt::Animation> getAnimation() const;
};

#endif // IMPACT_EXPLOSION_HPP_GUARD
