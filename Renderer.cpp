#include "Renderer.hpp"

Renderer::Renderer(std::shared_ptr<Device>& device) 
    : mDevice{device}
{   
    mSystem = std::make_shared<SimpleRenderSystem>(mDevice);
}

Renderer::~Renderer() 
{

}

void Renderer::PrepareSystems() const 
{
    mSystem->UpdateBuffers();
}

void Renderer::Run(const WGPUTextureView nextTexture) const
{
    WGPURenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = nextTexture;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
    renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
    renderPassColorAttachment.clearValue = WGPUColor{ 0.2, 0.4, 0.7, 1.0 };


    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.depthStencilAttachment = nullptr;
    renderPassDesc.timestampWriteCount = 0;
    renderPassDesc.timestampWrites = nullptr;
    renderPassDesc.nextInChain = nullptr;


    mDevice->InitCommandEncoder();
    WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(mDevice->GetEncoder(), &renderPassDesc);


    // Run each render system.
    mSystem->Run(renderPass);


    // Finish the render pass.
    wgpuRenderPassEncoderEnd(renderPass);


    // Store all the encoder commands in a buffer which gets sent to the gpu and realeased cpu side.
    //
    mDevice->InitCommandBuffer();
    const auto cmd = mDevice->GetCommandBuffer();

    // Submit the queue of commands to the gpu.
    wgpuQueueSubmit(mDevice->GetQueue(), 1, &cmd);


    // Release the resources for this frame.
    //
    wgpuRenderPassEncoderRelease(renderPass);
    wgpuTextureViewRelease(nextTexture);
    wgpuCommandEncoderRelease(mDevice->GetEncoder());
    wgpuCommandBufferRelease(cmd);
}   

