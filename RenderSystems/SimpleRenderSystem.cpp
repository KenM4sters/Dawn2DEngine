#include "SimpleRenderSystem.hpp"

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
    
}

void SimpleRenderSystem::Run(const WGPURenderPassEncoder encoder) 
{
    mPipeline->Set(encoder);
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
    cameraDesc.size = (16*sizeof(float)) + (16*sizeof(float)) + (4*sizeof(float)); // 2 mat4s, 1 vec3 + 4 bytes of padding.
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

    WGPUBindGroupLayoutEntry materialLayoutEntry;
    setDefault(&materialLayoutEntry); 
    materialLayoutEntry.binding = 0;
    materialLayoutEntry.visibility = WGPUShaderStage_Fragment;

    WGPUBindGroupLayoutEntry modelLayoutEntry;
    setDefault(&modelLayoutEntry); 
    modelLayoutEntry.binding = 0;
    modelLayoutEntry.visibility = WGPUShaderStage_Vertex;



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
    materialBindGroupEntry.binding = 1;
    materialBindGroupEntry.buffer = materialBuffer;
    materialBindGroupEntry.offset = 0;
    materialBindGroupEntry.size = materialDesc.size;

    WGPUBindGroupEntry modelBindGroupEntry{};
    modelBindGroupEntry.nextInChain = nullptr;  
    modelBindGroupEntry.binding = 2;
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


    // Bind Group.
    //
    WGPUBindGroupDescriptor cameraBindGroupDesc{};
    cameraBindGroupDesc.nextInChain = nullptr;
    cameraBindGroupDesc.layout = cameraLayout;
    // There must be as many bindings as declared in the layout!
    cameraBindGroupDesc.entryCount = 1;
    cameraBindGroupDesc.entries = &cameraBindGroupEntry;
    WGPUBindGroup cameraBindGroup = wgpuDeviceCreateBindGroup(mDevice->GetDevice(), &cameraBindGroupDesc);

    WGPUBindGroupDescriptor materialBindGroupDesc{};
    materialBindGroupDesc.nextInChain = nullptr;
    materialBindGroupDesc.layout = cameraLayout;
    // There must be as many bindings as declared in the layout!
    materialBindGroupDesc.entryCount = 1;
    materialBindGroupDesc.entries = &cameraBindGroupEntry;
    WGPUBindGroup materialBindGroup = wgpuDeviceCreateBindGroup(mDevice->GetDevice(), &materialBindGroupDesc);

    WGPUBindGroupDescriptor modelBindGroupDesc{};
    modelBindGroupDesc.nextInChain = nullptr;
    modelBindGroupDesc.layout = cameraLayout;
    // There must be as many bindings as declared in the layout!
    modelBindGroupDesc.entryCount = 1;
    modelBindGroupDesc.entries = &cameraBindGroupEntry;
    WGPUBindGroup modelBindGroup = wgpuDeviceCreateBindGroup(mDevice->GetDevice(), &modelBindGroupDesc);






    // Create the pipeline layout
    WGPUPipelineLayoutDescriptor layoutDesc{};
    layoutDesc.nextInChain = nullptr;
    layoutDesc.bindGroupLayoutCount = 1;
    layoutDesc.bindGroupLayouts = &bindGroupLayout;
    WGPUPipelineLayout layout = wgpuDeviceCreatePipelineLayout(device, &layoutDesc);

    mPipeline = std::make_unique<Pipeline>(
        mDevice,
        "Shaders/Simple.wgsl",
        layout,
        mGeometry->GetNativeBufferLayout()
    );
}
