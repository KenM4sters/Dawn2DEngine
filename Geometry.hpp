#pragma once
#include <webgpu/webgpu.hpp>
#include "Buffer.hpp"
#include "Device.hpp"

struct NativeBufferLayout 
{
    WGPUVertexBufferLayout Layout{};
    std::vector<WGPUVertexAttribute> Attributes{};
};

class Geometry 
{
    public:

        Geometry(std::shared_ptr<Device>& device);

        ~Geometry();

        inline const NativeBufferLayout& GetNativeBufferLayout() const { return mNativeLayout; }

        inline const WGPUBuffer& GetVertexBuffer() const { return mVertexBuffer; }

        inline const std::vector<float>& GetVertices() const { return mVertices; }

    private:
    
        void CreateBuffers(std::shared_ptr<Device>& device);

        std::vector<float>& mVertices;

        std::shared_ptr<BufferLayout> mBufferLayout = nullptr;

        WGPUBuffer mVertexBuffer = nullptr;

        NativeBufferLayout mNativeLayout{};

};