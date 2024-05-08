#include "SimpleRenderSystem.hpp"

SimpleRenderSystem::SimpleRenderSystem(std::shared_ptr<Device>& device) 
    : RenderSystem(device)
{
    CreatePipeline();
}


SimpleRenderSystem::~SimpleRenderSystem() 
{

}
 

void SimpleRenderSystem::WriteToBuffers() 
{

}


void SimpleRenderSystem::Run(WGPURenderPassEncoder encoder) 
{
    mPipeline->Set(encoder);
    wgpuRenderPassEncoderDraw(encoder, 3, 1, 0, 0);
}

void SimpleRenderSystem::CreatePipeline() 
{
    WGPUPipelineLayout layout{};

    mPipeline = std::make_unique<Pipeline>(
        mDevice,
        "Shaders/Simple.wgsl",
        layout
    );

}
