#pragma once
#include "../Program/Events/Bus.hpp"

class ISystem
{
public:
    ISystem() 
    {
        if(!mEventBus) 
        {
            mEventBus = std::make_shared<EventBus>();
        }
    }
    
    virtual ~ISystem() {}

    static std::shared_ptr<EventBus> mEventBus;
};


class IEntitySystem : public ISystem 
{
public:
    virtual void Run() = 0;
};

class IEventHandler : public ISystem 
{
public:
    
};