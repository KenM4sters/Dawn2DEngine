#pragma once
#include "RenderSystem.hpp"
#include "../Pipeline.hpp"

class SimpleRenderSystem : public RenderSystem 
{
    public:
        SimpleRenderSystem(std::shared_ptr<Device>& device);
        
        ~SimpleRenderSystem() override;

        void WriteToBuffers() override;

        void Run(WGPURenderPassEncoder encoder) override;

    private:
        
        void CreatePipeline();

        WGPUPipelineLayout mPipelineLayout;

        std::unique_ptr<Pipeline> mPipeline;
};