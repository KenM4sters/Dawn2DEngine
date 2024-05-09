#pragma once
#include <webgpu/webgpu.hpp>
#include "Buffer.hpp"
#include "Device.hpp"

struct NativeBufferLayout 
{
    WGPUVertexBufferLayout Layout{};
    std::vector<WGPUVertexAttribute> Attributes{};
};

struct UniformBufferObject
{
    WGPUBuffer UniformBuffer;
    WGPUBindGroup BindGroup;
};

class Geometry 
{
    public:

        Geometry(const std::shared_ptr<Device>& device);

        ~Geometry();

        inline const NativeBufferLayout& GetNativeBufferLayout() const { return mNativeLayout; }

        inline const WGPUBuffer& GetVertexBuffer() const { return mVertexBuffer; }

        inline const std::vector<float>& GetVertices() const { return mVertices; }

    private:
    
        void CreateBuffers(const std::shared_ptr<Device>& device);

        std::vector<float>& mVertices;

        std::shared_ptr<BufferLayout> mBufferLayout = nullptr;

        WGPUBuffer mVertexBuffer = nullptr;

        NativeBufferLayout mNativeLayout{};

};