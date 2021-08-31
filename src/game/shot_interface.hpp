#ifndef QUASARRUSH_SHOT_INTERFACE_HPP
#define QUASARRUSH_SHOT_INTERFACE_HPP

#include "transform.hpp"
#include <memory>

class ShotInterface
{
    virtual std::shared_ptr<Transform> getTransform() = 0;
    virtual bool getFiredByPlayer() const = 0;
    virtual void hit() = 0;
};

#endif // QUASARRUSH_SHOT_INTERFACE_HPP