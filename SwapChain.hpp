#pragma once
#include <GLFW/glfw3.h>
#include <webgpu/webgpu.h>

class SwapChain 
{
    public:
        SwapChain(WGPUDevice device, WGPUSurface surface);
        ~SwapChain();

        inline const WGPUSwapChain GetSwapChain() const { return mSwapChain; }

    private:

        WGPUSwapChain mSwapChain;
};