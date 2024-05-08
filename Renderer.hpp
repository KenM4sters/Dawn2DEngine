#pragma once
#include <glfw3webgpu.h>
#include <webgpu/webgpu.h>
#include "Device.hpp"
#include <iostream>
#include "RenderSystems/SimpleRenderSystem.hpp"

class Renderer 
{
    public:

        Renderer(std::shared_ptr<Device>& device);
        ~Renderer();

        void Run(WGPUTextureView nextTexture) const;
        
    private:
        
        std::shared_ptr<Device>& mDevice;
        
        std::shared_ptr<SimpleRenderSystem> mSystem = nullptr;
};