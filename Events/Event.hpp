#pragma once
#include <string>
#include "../ECS/Entity.hpp"

class IEvent 
{
public:
    virtual inline const std::string GetEventType() const = 0;

    bool mHandled = false;
};


class CollisionEvent : public IEvent 
{
public:
    CollisionEvent(Entity* A, Entity* B)
        : mEntityA{A}, mEntityB{B} 
    {}

    inline const std::string GetEventType() const override { return "CollisionEvent";}

    Entity* mEntityA = nullptr;
    Entity* mEntityB = nullptr;
};