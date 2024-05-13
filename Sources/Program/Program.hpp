#pragma once
#include <iostream>
#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "../Graphics/Renderer.hpp"
#include "../World/World.hpp"


typedef struct AdapterPayload 
{
    WGPUAdapter Adapter;
    WGPUSurface Surface;
} 
AdapterPayload;

class Program 
{
    public:
        Program(uint32_t w, uint32_t h, const char* label);
        ~Program();

        Program(const Program& other) = delete;
        Program& operator=(const Program& other) = delete;

        static Program& Get() { return *mInstance; }

        void Run() const;
        
        std::shared_ptr<Window> GetWindow() const { return mWindow; }
    
    private:

        AdapterPayload RequestAndInspectAdapter() const;
        
        std::shared_ptr<Window> mWindow = nullptr;
        std::shared_ptr<Device> mDevice = nullptr;
        std::shared_ptr<SwapChain> mSwapChain = nullptr;
        std::unique_ptr<Renderer> mRenderer = nullptr;
        std::unique_ptr<World> mWorld = nullptr;

        static Program* mInstance;
        
};