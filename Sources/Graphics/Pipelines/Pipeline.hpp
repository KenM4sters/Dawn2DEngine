#pragma once
#include <webgpu/webgpu.h>
#include <iostream>
#include <string>
#include <vector>

#include "../../Program/Device.hpp"
#include "../Buffers/Geometry.hpp"


class Pipeline 
{
    public:
        Pipeline(const std::shared_ptr<Device>& device, std::string shaderPath, WGPUPipelineLayout pipelineLayout, const NativeBufferLayout& bufferLayout);
        
        ~Pipeline();

        void Set(WGPURenderPassEncoder encoder) const { wgpuRenderPassEncoderSetPipeline(encoder, mRenderPipeline); }

    private:
        std::vector<char> ReadFromFile(const std::string& path);

        void CreateGraphicsPipeline(const std::string shaderPath, WGPUPipelineLayout layout, const NativeBufferLayout& bufferLayout);

        const std::shared_ptr<Device>& mDevice;

        WGPURenderPipeline mRenderPipeline;

        WGPUShaderModule mShaderModule;

};