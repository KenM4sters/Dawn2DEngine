#pragma once
#include <webgpu/webgpu.h>
#include <iostream>
#include <string>
#include <vector>

#include "Device.hpp"


class Pipeline 
{
    public:
        Pipeline(std::shared_ptr<Device>& device, std::string& shaderPath, WGPUPipelineLayout layout);
        ~Pipeline();

        void Set(WGPURenderPassEncoder encoder) const { wgpuRenderPassEncoderSetPipeline(encoder, mRenderPipeline); }

    private:
        std::vector<char> ReadFromFile(const std::string& path);

        void CreateGraphicsPipeline(const std::string& shaderPath, WGPUPipelineLayout layout);

        std::shared_ptr<Device>& mDevice;

        WGPURenderPipeline mRenderPipeline;

        WGPUShaderModule mVertexModules;
        WGPUShaderModule mFragmentModule;


};