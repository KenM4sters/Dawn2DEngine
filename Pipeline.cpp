#include "Pipeline.hpp"
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cassert>


Pipeline::Pipeline(const std::shared_ptr<Device>& device, std::string shaderPath, WGPUPipelineLayout layout, const NativeBufferLayout& bufferLayout) 
    : mDevice{device}
{
    CreateGraphicsPipeline(shaderPath, layout, bufferLayout);
}

Pipeline::~Pipeline() 
{
    wgpuRenderPipelineRelease(mRenderPipeline);
    wgpuShaderModuleRelease(mShaderModule);
}

std::vector<char> Pipeline::ReadFromFile(const std::string path) 
{
    std::ifstream file{path, std::ios::ate | std::ios::binary}; 

    if(!file.is_open()) {
        throw std::runtime_error("Couldn't open file: " + path);
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

void Pipeline::CreateGraphicsPipeline(const std::string shaderPath, WGPUPipelineLayout layout, const NativeBufferLayout& bufferLayout) 
{
    auto shaderSrc = ReadFromFile(shaderPath);

    WGPUShaderModuleDescriptor shaderDesc{};
    WGPUShaderModuleWGSLDescriptor shaderCodeDesc{};
    shaderCodeDesc.chain.next = nullptr;
    shaderCodeDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
    shaderDesc.nextInChain = &shaderCodeDesc.chain;
    shaderCodeDesc.code = reinterpret_cast<const char*>(shaderSrc.data());
    mShaderModule = wgpuDeviceCreateShaderModule(mDevice->GetDevice(), &shaderDesc);

    WGPUBlendState blendState{};
    blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
    blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
    blendState.color.operation = WGPUBlendOperation_Add;
    blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
    blendState.alpha.dstFactor = WGPUBlendFactor_One;
    blendState.alpha.operation = WGPUBlendOperation_Add;

    WGPUColorTargetState colorTarget{};
    colorTarget.format = WGPUTextureFormat_BGRA8Unorm; // !! Must be the same format as the Swap Chain.
    colorTarget.blend = &blendState;
    colorTarget.writeMask = WGPUColorWriteMask_All; // We could write to only some of the color channels.

    WGPUFragmentState fragmentState{};
    fragmentState.module = mShaderModule;
    fragmentState.entryPoint = "fs_main";
    fragmentState.constantCount = 0;
    fragmentState.constants = nullptr;
    fragmentState.targetCount = 1; // we're only using one color attachment. 
    fragmentState.targets = &colorTarget;

    WGPURenderPipelineDescriptor pipelineDesc{};
    pipelineDesc.nextInChain = nullptr;
    pipelineDesc.vertex.bufferCount = 1;
    pipelineDesc.vertex.buffers = &bufferLayout.Layout;
    pipelineDesc.vertex.module = mShaderModule;
    pipelineDesc.vertex.entryPoint = "vs_main";
    pipelineDesc.vertex.constantCount = 0;
    pipelineDesc.vertex.constants = nullptr;
    pipelineDesc.primitive.topology = WGPUPrimitiveTopology_TriangleList;
    pipelineDesc.primitive.stripIndexFormat = WGPUIndexFormat_Undefined;
    pipelineDesc.primitive.frontFace = WGPUFrontFace_CCW;
    pipelineDesc.primitive.cullMode = WGPUCullMode_None;
    pipelineDesc.multisample.count = 1; // Samples per pixel - used for antialaising.
    pipelineDesc.multisample.mask = ~0u;
    pipelineDesc.multisample.alphaToCoverageEnabled = false;

    pipelineDesc.fragment = &fragmentState;
    pipelineDesc.layout = nullptr;

    mRenderPipeline = wgpuDeviceCreateRenderPipeline(mDevice->GetDevice(), &pipelineDesc);
}