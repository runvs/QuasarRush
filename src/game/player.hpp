#ifndef GAME_PLAYER_GUARD_HPP
#define GAME_PLAYER_GUARD_HPP

#include "game_object.hpp"
#include "shape.hpp"
#include "sprite.hpp"
#include "transform.hpp"
#include <memory>

class Player : public jt::GameObject {
private:
    std::shared_ptr<jt::Sprite> m_sprite;
    std::shared_ptr<jt::Shape> m_projectionShape;
    std::shared_ptr<Transform> m_transform;
    std::vector<jt::Vector2> m_projectionPoints;
    
    void updateMovement(const float elapsed);

public:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    std::shared_ptr<Transform> getTransform();
    void setTransform(std::shared_ptr<Transform> t);

    void setProjectionPoints(std::vector<jt::Vector2>&& points);
};

#endif
