#ifndef PLANET_GUARD_HPP
#define PLANET_GUARD_HPP

#include "game_object.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>

class Planet : public jt::GameObject {
private:
    std::shared_ptr<jt::Sprite> m_sprite;
    std::shared_ptr<Transform> m_transform;

public:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    std::shared_ptr<Transform> getTransform();
    void setTransform(std::shared_ptr<Transform> t);
};

#endif // PLANET_GUARD_HPP
