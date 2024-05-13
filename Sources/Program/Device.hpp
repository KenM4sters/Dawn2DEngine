#pragma once
#include <webgpu/webgpu.h>


class Device 
{
    public:
        Device(const WGPUAdapter adapter);
        
        ~Device();

        const WGPUDevice GetDevice() const { return mGPU; }

        const WGPUQueue GetQueue() const { return mQueue; } 
        
        const WGPUCommandEncoder GetEncoder() const { return mEncoder; }
        
        const WGPUCommandBuffer GetCommandBuffer() const { return mCommandBuffer; }
        
        void InitCommandEncoder();
        
        void InitCommandBuffer();
        
    private:

        WGPUDevice mGPU;

        WGPUQueue mQueue;

        WGPUCommandEncoder mEncoder;

        WGPUCommandBuffer mCommandBuffer;
        
        WGPUAdapter mAdapter;
};