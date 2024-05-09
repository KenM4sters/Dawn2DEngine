#pragma once
#include <iostream>
#include "../Device.hpp"

class RenderSystem 
{
    public:

        RenderSystem(std::shared_ptr<Device>& device)
            : mDevice{device}
        {

        }

        virtual ~RenderSystem() {}
        
        virtual void WriteToBuffers() = 0;

        virtual void Run(WGPURenderPassEncoder encoder) = 0;

    protected:

        std::shared_ptr<Device>& mDevice;

};