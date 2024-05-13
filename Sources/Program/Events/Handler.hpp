#pragma once
#include "./Event.hpp"


// Base class.
//
class IHandler 
{
public:
    inline void Execute(IEvent* event) { Call(event); }

protected:
    virtual void Call(IEvent* event) = 0;
};



// Derived class.
//
template<class T, class EventType>
class Handler : public IHandler 
{
public:
   typedef void (T::*MemberFunction)(EventType*);
    Handler(T* instance, MemberFunction memberFunction)
        : mInstance{instance}, mMemberFunction{memberFunction} 
    {}

    void Call(IEvent* event) override 
    {
        (mInstance->*mMemberFunction)(static_cast<EventType*>(event));
    }


private:
    T* mInstance;
    MemberFunction mMemberFunction;
};

