#include <iostream>
#include "SwapChain.hpp"


SwapChain::SwapChain(WGPUDevice device, WGPUSurface surface) 
{
    WGPUSwapChainDescriptor swapChainDesc = {};
    swapChainDesc.nextInChain = nullptr;
    swapChainDesc.width = 640;
    swapChainDesc.height = 480;
    swapChainDesc.format = WGPUTextureFormat_BGRA8Unorm;
    swapChainDesc.usage = WGPUTextureUsage_RenderAttachment;
    swapChainDesc.presentMode = WGPUPresentMode_Fifo;

    mSwapChain = wgpuDeviceCreateSwapChain(device, surface, &swapChainDesc);
    std::cout << "Swapchain: " << mSwapChain << std::endl;
}

SwapChain::~SwapChain() 
{

}