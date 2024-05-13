#include "Systems.hpp"
#include "../Program/Events/Event.hpp"

class CollisionHandler : public IEventHandler 
{
public:
    CollisionHandler() 
    {
        mEventBus->Subscribe(this, &CollisionHandler::OnCollisionEvent);
    }

    void OnCollisionEvent(CollisionEvent* collision) 
    {
        collision->mEntityA->flags &= ~Entity_Flags::ACTIVE;
    }
};