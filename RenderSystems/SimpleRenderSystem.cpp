#include "SimpleRenderSystem.hpp"
#include "../World.hpp"

void setDefault(WGPUBindGroupLayoutEntry* bindGroupEntry) 
{
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
    : RenderSystem(device), mEntities(World::GetEntities())
{
    for(auto& ent : mEntities) 
    {
        // Geometry
        ent.renderable.geometry = new Geometry(device);

        SetDefaultRenderingData(&ent);

        wgpuQueueWriteBuffer(mDevice->GetQueue(), ent.renderable.geometry->GetVertexBuffer(), 0, ent.renderable.geometry->GetVertices().data(), ent.renderable.geometry->GetVertices().size()*sizeof(float));
    }
}


SimpleRenderSystem::~SimpleRenderSystem() 
{
}
 

void SimpleRenderSystem::UpdateBuffers() 
{

    for(auto& ent : mEntities) 
    {
        const auto& uniforms = ent.renderable.uniforms;

        ent.world_transform.model_matrix = glm::mat4(1.0f);
        ent.world_transform.model_matrix = glm::translate(ent.world_transform.model_matrix, ent.world_transform.position);
        ent.world_transform.model_matrix = glm::scale(ent.world_transform.model_matrix, glm::vec3(ent.bounds.width , ent.bounds.height, 1.0f));

        wgpuQueueWriteBuffer(mDevice->GetQueue(), uniforms[0].UniformBuffer, 0, &World::GetCamera()->GetProjectionMatrix(), 16*sizeof(float));
        wgpuQueueWriteBuffer(mDevice->GetQueue(), uniforms[0].UniformBuffer, 16*sizeof(float), &World::GetCamera()->GetViewMatrix(), 16*sizeof(float));
        wgpuQueueWriteBuffer(mDevice->GetQueue(), uniforms[0].UniformBuffer, 32*sizeof(float), &World::GetCamera()->GetPosition(), 3*sizeof(float));

        wgpuQueueWriteBuffer(mDevice->GetQueue(), uniforms[1].UniformBuffer, 0, &ent.color, 3*sizeof(float));
        wgpuQueueWriteBuffer(mDevice->GetQueue(), uniforms[2].UniformBuffer, 0, &ent.world_transform.model_matrix, 16*sizeof(float));
    }
}

void SimpleRenderSystem::Run(const WGPURenderPassEncoder encoder) 
{
    mPipeline->Set(encoder);

    for(auto& ent : mEntities) 
    {
        if(ent.flags & Entity_Flags::ACTIVE) 
        {
            const auto& renderable = ent.renderable;
            wgpuRenderPassEncoderSetBindGroup(encoder, 0, renderable.uniforms[0].BindGroup, 0, nullptr);
            wgpuRenderPassEncoderSetBindGroup(encoder, 1, renderable.uniforms[1].BindGroup, 0, nullptr);
            wgpuRenderPassEncoderSetBindGroup(encoder, 2, renderable.uniforms[2].BindGroup, 0, nullptr);

            wgpuRenderPassEncoderSetVertexBuffer(encoder, 0, renderable.geometry->GetVertexBuffer(), 0, renderable.geometry->GetVertices().size()*sizeof(float));
            
            wgpuRenderPassEncoderDraw(encoder, 6, 1, 0, 0);

            ent.velocity = glm::vec3{0.0f};
            ent.acceleration = glm::vec3{0.0f};
        }
    }
}

void SimpleRenderSystem::SetDefaultRenderingData(Entity* ent) 
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
    cameraLayoutEntry.nextInChain = nullptr;
    cameraLayoutEntry.binding = 0;
    cameraLayoutEntry.visibility = WGPUShaderStage_Vertex;
    cameraLayoutEntry.buffer.minBindingSize = 144;
    cameraLayoutEntry.buffer.type = WGPUBufferBindingType_Uniform;

    WGPUBindGroupLayoutEntry materialLayoutEntry;
    setDefault(&materialLayoutEntry); 
    materialLayoutEntry.nextInChain = nullptr;
    materialLayoutEntry.binding = 0;
    materialLayoutEntry.visibility = WGPUShaderStage_Fragment;
    materialLayoutEntry.buffer.minBindingSize = 3*sizeof(float);
    materialLayoutEntry.buffer.type = WGPUBufferBindingType_Uniform;

    WGPUBindGroupLayoutEntry modelLayoutEntry;
    setDefault(&modelLayoutEntry); 
    modelLayoutEntry.nextInChain = nullptr;
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


    // Push data into the entity's renderable member.
    //
    ent->renderable.uniforms.push_back({cameraBuffer, cameraBindGroup});
    ent->renderable.uniforms.push_back({materialBuffer, materialBindGroup});
    ent->renderable.uniforms.push_back({modelBuffer, modelBindGroup});

    if(!mPipeline) 
    {
        // Create the pipeline layout
        //
        WGPUPipelineLayoutDescriptor layoutDesc{};
        layoutDesc.nextInChain = nullptr;
        layoutDesc.bindGroupLayoutCount = 3;
        layoutDesc.bindGroupLayouts = bindGroupLayouts.data();
        WGPUPipelineLayout layout = wgpuDeviceCreatePipelineLayout(mDevice->GetDevice(), &layoutDesc);

        // Instantiate the pipeline with the pipeline layout.
        //
        mPipeline = std::make_unique<Pipeline>(
            mDevice,
            "Shaders/Simple.wgsl",
            layout,
            mEntities[0].renderable.geometry->GetNativeBufferLayout()
        );
    }
}
