#pragma once
#include <iostream>
#include <unordered_map>
#include "../../Program/Device.hpp"
#include "../Pipelines/Pipeline.hpp"

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

        const std::unordered_map<std::string, Pipeline*> mPipelines{};
        

};