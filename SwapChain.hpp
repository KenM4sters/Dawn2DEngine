#pragma once
#include <glfw/glfw3.h>
#include <webgpu/webgpu.h>

class SwapChain 
{
    public:
        SwapChain(WGPUDevice device, WGPUSurface surface);
        ~SwapChain();

        const WGPUSwapChain GetSwapChain() const { return mSwapChain; }

    private:

        WGPUSwapChain mSwapChain;
};