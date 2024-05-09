#pragma once
#include <iostream>
#include "../Device.hpp"

class RenderSystem 
{
    public:

        RenderSystem(const std::shared_ptr<Device>& device)
            : mDevice{device}
        {

        }

        virtual ~RenderSystem() {}
        
        virtual void UpdateBuffers() = 0;

        virtual void Run(WGPURenderPassEncoder encoder) = 0;

    protected:

        const std::shared_ptr<Device>& mDevice;

};