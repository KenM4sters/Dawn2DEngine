#include "Program.hpp"
#include <webgpu/webgpu.h>

Program::Program(uint32_t w, uint32_t h, const char* label) 
{
    mWindow = std::make_shared<Window>(w, h, label);

    WGPUInstanceDescriptor desc = {};
    desc.nextInChain = nullptr;

    WGPUInstance instance = wgpuCreateInstance(&desc);
}

Program::~Program() 
{

}

void Program::Run() const
{
    while(!mWindow->WindowShouldClose()) 
    {
        glfwPollEvents();
    }
}