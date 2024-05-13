#pragma once
#include <iostream>
#include <glfw3webgpu.h>
#include <webgpu/webgpu.h>
#include "../Program/Device.hpp"
#include "./Systems/SimpleRenderSystem.hpp"

class Renderer 
{
    public:

        Renderer(std::shared_ptr<Device>& device);
        
        ~Renderer();

        void PrepareRenderSystem() const;

        void Run(const WGPUTextureView nextTexture) const;
        
    private:
        
        std::shared_ptr<Device>& mDevice;
        
        std::shared_ptr<SimpleRenderSystem> mSystem = nullptr;
};