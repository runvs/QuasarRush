#ifndef QUASARRUSH_TARGET_HPP
#define QUASARRUSH_TARGET_HPP

#include "animation.hpp"
#include "game_object.hpp"
#include "sprite.hpp"
#include "tween_base.hpp"

class Target : public jt::GameObject {
public:
    explicit Target(jt::Vector2 const& pos);
    jt::Vector2 getPosition() const;

    std::vector<std::shared_ptr<jt::TweenBase>> hit();

    bool hasBeenHit() const;
    std::shared_ptr<jt::Animation> getAnimation() const;

private:
    std::shared_ptr<jt::Animation> m_animation;
    std::shared_ptr<jt::Sprite> m_glow;
    jt::Vector2 m_position;
    bool m_hasBeenHit { false };

    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;
};

#endif // QUASARRUSH_TARGET_HPP
