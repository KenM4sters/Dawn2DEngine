#pragma once

class ISystem
{
public:
    ISystem() 
    {

    }
    virtual ~ISystem() {}

    virtual void Run() = 0;

};