#pragma once
#include "../Events/Bus.hpp"

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

    virtual void Run() = 0;

    static std::shared_ptr<EventBus> mEventBus;
};