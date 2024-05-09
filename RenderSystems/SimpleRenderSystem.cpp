#include "SimpleRenderSystem.hpp"

SimpleRenderSystem::SimpleRenderSystem(std::shared_ptr<Device>& device) 
    : RenderSystem(device)
{
    mGeometry = new Geometry(mDevice);
    CreatePipeline();
    wgpuQueueWriteBuffer(mDevice->GetQueue(), mGeometry->GetVertexBuffer(), 0, mGeometry->GetVertices().data(), mGeometry->GetVertices().size()*sizeof(float));

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
    wgpuRenderPassEncoderSetVertexBuffer(encoder, 0, mGeometry->GetVertexBuffer(), 0, mGeometry->GetVertices().size()*sizeof(float));
    wgpuRenderPassEncoderDraw(encoder, 6, 1, 0, 0);
}

void SimpleRenderSystem::CreatePipeline() 
{
    WGPUPipelineLayout layout{};

    mPipeline = std::make_unique<Pipeline>(
        mDevice,
        "Shaders/Simple.wgsl",
        layout,
        mGeometry->GetNativeBufferLayout()
    );
}
