#pragma once
#include "RenderSystem.hpp"
#include "../Pipeline.hpp"
#include "../Geometry.hpp"

class SimpleRenderSystem : public RenderSystem 
{
    public:
        SimpleRenderSystem(const std::shared_ptr<Device>& device);
        
        ~SimpleRenderSystem() override;

        void UpdateBuffers() override;

        void Run(WGPURenderPassEncoder encoder) override;

    private:
        
        void CreatePipeline();

        WGPUPipelineLayout mPipelineLayout;

        std::unique_ptr<Pipeline> mPipeline;

        Geometry* mGeometry = nullptr;
};