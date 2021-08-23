#ifndef GAME_PLAYER_GUARD_HPP
#define GAME_PLAYER_GUARD_HPP

#include "shape.hpp"
#include "game_object.hpp"
#include "transform.hpp"
#include <memory>

class Player : public jt::GameObject {
private:
    std::shared_ptr<jt::Shape> m_shape;
    std::shared_ptr<Transform> m_transform;
    
public:
    void doCreate() override;
    void doUpdate(float const /*elapsed*/) override;
    void doDraw() const override;
    void doKill() override;

    std::shared_ptr<Transform> getTransform();
};
	    

#endif 