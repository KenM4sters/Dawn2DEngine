#pragma once
#include "RenderSystem.hpp"

class SimpleRenderSystem : public RenderSystem 
{
    public:
        SimpleRenderSystem(std::shared_ptr<Device>& device);
        
        ~SimpleRenderSystem() override;

        void Init() override;

        void WriteToBuffers() override;

        void Run() override;

};