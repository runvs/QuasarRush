#ifndef SHOT_GUARD_HPP
#define SHOT_GUARD_HPP

#include "game_object.hpp"
#include "shape.hpp"
#include "transform.hpp"
#include <memory>

class Shot : public jt::GameObject {
private:
    std::shared_ptr<jt::Shape> m_shape;
    std::shared_ptr<Transform> m_transform;

public:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    std::shared_ptr<Transform> getTransform();
    void setTransform(std::shared_ptr<Transform> t);
};

#endif
