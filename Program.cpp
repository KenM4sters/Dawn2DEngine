#include <webgpu/webgpu.h>
#include <vector>
#include <cassert>

#include "Program.hpp"

/**
 * Utility function to get a WebGPU adapter, so that
 *     WGPUAdapter adapter = requestAdapter(options);
 * is roughly equivalent to
 *     const adapter = await navigator.gpu.requestAdapter(options);
 */
WGPUAdapter requestAdapter(WGPUInstance instance, WGPURequestAdapterOptions const * options) 
{
    // A simple structure holding the local information shared with the
    // onAdapterRequestEnded callback.
    struct UserData {
        WGPUAdapter adapter = nullptr;
        bool requestEnded = false;
    };
    UserData userData;

    // Callback called by wgpuInstanceRequestAdapter when the request returns
    // This is a C++ lambda function, but could be any function defined in the
    // global scope. It must be non-capturing (the brackets [] are empty) so
    // that it behaves like a regular C function pointer, which is what
    // wgpuInstanceRequestAdapter expects (WebGPU being a C API). The workaround
    // is to convey what we want to capture through the pUserData pointer,
    // provided as the last argument of wgpuInstanceRequestAdapter and received
    // by the callback as its last argument.
    auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const * message, void * pUserData) 
    {
        UserData& userData = *reinterpret_cast<UserData*>(pUserData);

        if (status == WGPURequestAdapterStatus_Success) 
        {
            userData.adapter = adapter;
        } else 
        {
            std::cout << "Could not get WebGPU adapter: " << message << std::endl;
        }

        userData.requestEnded = true;
    };

    // Call to the WebGPU request adapter procedure
    wgpuInstanceRequestAdapter(
        instance,
        options,
        onAdapterRequestEnded,
        (void*)&userData
    );

    // When this funciton returns, we know that the callback() has been called and returned.
    //
    assert(userData.requestEnded);

    return userData.adapter;
}


Program::Program(uint32_t w, uint32_t h, const char* label) 
{
    mWindow = std::make_shared<Window>(w, h, label);

    AdapterPayload p = RequestAndInspectAdapter();
    
    mDevice = std::make_shared<Device>(p.Adapter);

    mSwapChain = std::make_shared<SwapChain>(mDevice->GetDevice(), p.Surface);

}

Program::~Program() 
{
    
}

void Program::Run() const
{
    while(!mWindow->WindowShouldClose()) 
    {
        glfwPollEvents();
        
        // Get the current texture view that we'll render to from the swap chain.
        WGPUTextureView nextTexture = wgpuSwapChainGetCurrentTextureView(mSwapChain->GetSwapChain()); 
        assert(nextTexture);

        mDevice->InitCommandEncoder();

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


        WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(mDevice->GetEncoder(), &renderPassDesc);
        wgpuRenderPassEncoderEnd(renderPass);
        wgpuRenderPassEncoderRelease(renderPass);

        wgpuTextureViewRelease(nextTexture);

        mDevice->InitCommandBuffer();

        const auto cmd = mDevice->GetCommandBuffer();

        wgpuCommandEncoderRelease(mDevice->GetEncoder());
		wgpuQueueSubmit(mDevice->GetQueue(), 1, &cmd);
		wgpuCommandBufferRelease(cmd);

        wgpuSwapChainPresent(mSwapChain->GetSwapChain());
    }
}

AdapterPayload Program::RequestAndInspectAdapter() const 
{

    // Create an Instance - similar to navigator.gpu for the web.
    // 
    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;
    WGPUInstance instance = wgpuCreateInstance(&desc);

    std::cout << "Requesting adapter..." << std::endl;

    WGPURequestAdapterOptions adapterOpts = {};
    WGPUAdapter adapter = requestAdapter(instance, &adapterOpts);

    std::cout << "Adapter recieved: " << adapter << std::endl;

    // Surface.
    WGPUSurface surface = mWindow->GetWindowSurface(instance);


    // Evaluate the capabilities of the gpu adapter.
    //
    std::vector<WGPUFeatureName> features;

    // Call the function a first time with a null return address, just to get
    // the entry count.
    size_t featureCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);

    // Allocate memory (could be a new, or a malloc() if this were a C program)
    features.resize(featureCount);

    // Call the function a second time, with a non-null return address
    wgpuAdapterEnumerateFeatures(adapter, features.data());

    std::cout << "Adapter features:" << std::endl;

    for (auto f : features) 
    {
        std::cout << " - " << f << std::endl;
    }

    return {adapter, surface};

}