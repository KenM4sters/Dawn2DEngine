#include "SimpleRenderSystem.hpp"
#include "../AssetManager.hpp"

void setDefault(WGPUBindGroupLayoutEntry* bindGroupEntry) {
    bindGroupEntry->buffer.nextInChain = nullptr;
    bindGroupEntry->buffer.type = WGPUBufferBindingType_Undefined;
    bindGroupEntry->buffer.hasDynamicOffset = false;

    bindGroupEntry->sampler.nextInChain = nullptr;
    bindGroupEntry->sampler.type = WGPUSamplerBindingType_Undefined;

    bindGroupEntry->storageTexture.nextInChain = nullptr;
    bindGroupEntry->storageTexture.access = WGPUStorageTextureAccess_Undefined;
    bindGroupEntry->storageTexture.format = WGPUTextureFormat_Undefined;
    bindGroupEntry->storageTexture.viewDimension = WGPUTextureViewDimension_Undefined;

    bindGroupEntry->texture.nextInChain = nullptr;
    bindGroupEntry->texture.multisampled = false;
    bindGroupEntry->texture.sampleType = WGPUTextureSampleType_Undefined;
    bindGroupEntry->texture.viewDimension = WGPUTextureViewDimension_Undefined;
}

SimpleRenderSystem::SimpleRenderSystem(const std::shared_ptr<Device>& device) 
    : RenderSystem(device)
{
    mGeometry = new Geometry(mDevice);
    CreatePipeline();
    wgpuQueueWriteBuffer(mDevice->GetQueue(), mGeometry->GetVertexBuffer(), 0, mGeometry->GetVertices().data(), mGeometry->GetVertices().size()*sizeof(float));

}


SimpleRenderSystem::~SimpleRenderSystem() 
{
    delete mGeometry;
}
 

void SimpleRenderSystem::UpdateBuffers() 
{
    mModel = glm::mat4(1.0f);
    mModel = glm::translate(mModel, glm::vec3(375.0f, 275.0f, 0.0f));
    mModel = glm::scale(mModel, glm::vec3(50.0f, 50.0f, 1.0f));

    wgpuQueueWriteBuffer(mDevice->GetQueue(), mUniformBuffers[0].UniformBuffer, 0, &AssetManager::mCamera->GetProjectionMatrix(), 16*sizeof(float));
    wgpuQueueWriteBuffer(mDevice->GetQueue(), mUniformBuffers[0].UniformBuffer, 16*sizeof(float), &AssetManager::mCamera->GetViewMatrix(), 16*sizeof(float));
    wgpuQueueWriteBuffer(mDevice->GetQueue(), mUniformBuffers[0].UniformBuffer, 32*sizeof(float), &AssetManager::mCamera->GetPosition(), 3*sizeof(float));

    wgpuQueueWriteBuffer(mDevice->GetQueue(), mUniformBuffers[1].UniformBuffer, 0, &mColor, 3*sizeof(float));
    wgpuQueueWriteBuffer(mDevice->GetQueue(), mUniformBuffers[2].UniformBuffer, 0, &mModel, 16*sizeof(float));
}

void SimpleRenderSystem::Run(const WGPURenderPassEncoder encoder) 
{
    mPipeline->Set(encoder);
    wgpuRenderPassEncoderSetBindGroup(encoder, 0, mUniformBuffers[0].BindGroup, 0, nullptr);
    wgpuRenderPassEncoderSetBindGroup(encoder, 1, mUniformBuffers[1].BindGroup, 0, nullptr);
    wgpuRenderPassEncoderSetBindGroup(encoder, 2, mUniformBuffers[2].BindGroup, 0, nullptr);
    wgpuRenderPassEncoderSetVertexBuffer(encoder, 0, mGeometry->GetVertexBuffer(), 0, mGeometry->GetVertices().size()*sizeof(float));
    wgpuRenderPassEncoderDraw(encoder, 6, 1, 0, 0);
}

void SimpleRenderSystem::CreatePipeline() 
{

    // Uniform Buffers.
    //
    // Create camera buffer
    WGPUBufferDescriptor cameraDesc{};
    cameraDesc.nextInChain = nullptr;
    cameraDesc.size = 144; // 2 mat4s, 1 vec3 + 4 bytes of padding.
    cameraDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
    cameraDesc.mappedAtCreation = false;
    WGPUBuffer cameraBuffer = wgpuDeviceCreateBuffer(mDevice->GetDevice(), &cameraDesc);

    // Create material buffer
    WGPUBufferDescriptor materialDesc{};
    materialDesc.nextInChain = nullptr;
    materialDesc.size = (3*sizeof(float));
    materialDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
    materialDesc.mappedAtCreation = false;
    WGPUBuffer materialBuffer = wgpuDeviceCreateBuffer(mDevice->GetDevice(), &materialDesc);

    // Create model buffer
    WGPUBufferDescriptor modelDesc{};
    modelDesc.nextInChain = nullptr;
    modelDesc.size = (16*sizeof(float));
    modelDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Uniform;
    modelDesc.mappedAtCreation = false;
    WGPUBuffer modelBuffer = wgpuDeviceCreateBuffer(mDevice->GetDevice(), &modelDesc);



    // Bind Group Layout Entries.
    //
    WGPUBindGroupLayoutEntry cameraLayoutEntry;
    setDefault(&cameraLayoutEntry); 
    cameraLayoutEntry.binding = 0;
    cameraLayoutEntry.visibility = WGPUShaderStage_Vertex;
    cameraLayoutEntry.buffer.minBindingSize = 144;
    cameraLayoutEntry.buffer.type = WGPUBufferBindingType_Uniform;

    WGPUBindGroupLayoutEntry materialLayoutEntry;
    setDefault(&materialLayoutEntry); 
    materialLayoutEntry.binding = 0;
    materialLayoutEntry.visibility = WGPUShaderStage_Fragment;
    materialLayoutEntry.buffer.minBindingSize = 3*sizeof(float);
    materialLayoutEntry.buffer.type = WGPUBufferBindingType_Uniform;

    WGPUBindGroupLayoutEntry modelLayoutEntry;
    setDefault(&modelLayoutEntry); 
    modelLayoutEntry.binding = 0;
    modelLayoutEntry.visibility = WGPUShaderStage_Vertex;
    modelLayoutEntry.buffer.minBindingSize = 16*sizeof(float);
    modelLayoutEntry.buffer.type = WGPUBufferBindingType_Uniform;



    // Bind Group Entries.
    //
    WGPUBindGroupEntry cameraBindGroupEntry{};
    cameraBindGroupEntry.nextInChain = nullptr;  
    cameraBindGroupEntry.binding = 0;
    cameraBindGroupEntry.buffer = cameraBuffer;
    cameraBindGroupEntry.offset = 0;
    cameraBindGroupEntry.size = cameraDesc.size;

    WGPUBindGroupEntry materialBindGroupEntry{};
    materialBindGroupEntry.nextInChain = nullptr;  
    materialBindGroupEntry.binding = 0;
    materialBindGroupEntry.buffer = materialBuffer;
    materialBindGroupEntry.offset = 0;
    materialBindGroupEntry.size = materialDesc.size;

    WGPUBindGroupEntry modelBindGroupEntry{};
    modelBindGroupEntry.nextInChain = nullptr;  
    modelBindGroupEntry.binding = 0;
    modelBindGroupEntry.buffer = modelBuffer;
    modelBindGroupEntry.offset = 0;
    modelBindGroupEntry.size = modelDesc.size;



    // Bind Group Layout
    //
    WGPUBindGroupLayoutDescriptor cameraLayoutDesc{};
    cameraLayoutDesc.nextInChain = nullptr;
    cameraLayoutDesc.entryCount = 1;
    cameraLayoutDesc.entries = &cameraLayoutEntry;
    WGPUBindGroupLayout cameraLayout = wgpuDeviceCreateBindGroupLayout(mDevice->GetDevice(), &cameraLayoutDesc);

    WGPUBindGroupLayoutDescriptor materialLayoutDesc{};
    materialLayoutDesc.nextInChain = nullptr;
    materialLayoutDesc.entryCount = 1;
    materialLayoutDesc.entries = &materialLayoutEntry;
    WGPUBindGroupLayout materialLayout = wgpuDeviceCreateBindGroupLayout(mDevice->GetDevice(), &materialLayoutDesc);

    WGPUBindGroupLayoutDescriptor modelLayoutDesc{};
    modelLayoutDesc.nextInChain = nullptr;
    modelLayoutDesc.entryCount = 1;
    modelLayoutDesc.entries = &modelLayoutEntry;
    WGPUBindGroupLayout modelLayout = wgpuDeviceCreateBindGroupLayout(mDevice->GetDevice(), &modelLayoutDesc);

    std::vector<WGPUBindGroupLayout> bindGroupLayouts;
    bindGroupLayouts.push_back(cameraLayout);
    bindGroupLayouts.push_back(materialLayout);
    bindGroupLayouts.push_back(modelLayout);

    // Bind Group.
    //
    WGPUBindGroupDescriptor cameraBindGroupDesc{};
    cameraBindGroupDesc.nextInChain = nullptr;
    cameraBindGroupDesc.layout = cameraLayout;
    cameraBindGroupDesc.entryCount = 1;
    cameraBindGroupDesc.entries = &cameraBindGroupEntry;
    WGPUBindGroup cameraBindGroup = wgpuDeviceCreateBindGroup(mDevice->GetDevice(), &cameraBindGroupDesc);

    WGPUBindGroupDescriptor materialBindGroupDesc{};
    materialBindGroupDesc.nextInChain = nullptr;
    materialBindGroupDesc.layout = materialLayout;
    materialBindGroupDesc.entryCount = 1;
    materialBindGroupDesc.entries = &materialBindGroupEntry;
    WGPUBindGroup materialBindGroup = wgpuDeviceCreateBindGroup(mDevice->GetDevice(), &materialBindGroupDesc);

    WGPUBindGroupDescriptor modelBindGroupDesc{};
    modelBindGroupDesc.nextInChain = nullptr;
    modelBindGroupDesc.layout = modelLayout;
    modelBindGroupDesc.entryCount = 1;
    modelBindGroupDesc.entries = &modelBindGroupEntry;
    WGPUBindGroup modelBindGroup = wgpuDeviceCreateBindGroup(mDevice->GetDevice(), &modelBindGroupDesc);


    // Create the pipeline layout
    WGPUPipelineLayoutDescriptor layoutDesc{};
    layoutDesc.nextInChain = nullptr;
    layoutDesc.bindGroupLayoutCount = 3;
    layoutDesc.bindGroupLayouts = bindGroupLayouts.data();
    WGPUPipelineLayout layout = wgpuDeviceCreatePipelineLayout(mDevice->GetDevice(), &layoutDesc);

    // Push the buffers and bind groups into the member container for writing each frame.
    //
    UniformBufferObject cameraUBO = {cameraBuffer, cameraBindGroup};
    UniformBufferObject materialUBO = {materialBuffer, materialBindGroup};
    UniformBufferObject modelUBO = {modelBuffer, modelBindGroup};

    mUniformBuffers.push_back(cameraUBO);
    mUniformBuffers.push_back(materialUBO);
    mUniformBuffers.push_back(modelUBO);

    // Instantiate the pipeline with the pipeline layout.
    //
    mPipeline = std::make_unique<Pipeline>(
        mDevice,
        "Shaders/Simple.wgsl",
        layout,
        mGeometry->GetNativeBufferLayout()
    );
}
