#include "Systems.hpp"
#include "../Events/Event.hpp"

class CollisionSystem : public ISystem 
{
public:
    CollisionSystem() 
    {
        mEventBus->Subscribe(this, &CollisionSystem::OnCollisionEvent);
    }

    void OnCollisionEvent(CollisionEvent* collision) 
    {
        collision->mEntityA->flags &= ~Entity_Flags::ACTIVE;
    }

    void Run() override {}
};