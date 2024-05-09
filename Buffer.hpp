#include <webgpu.h>
#include <vector>
#include <cassert>

class BufferAttribute  
{
    public:

        BufferAttribute(uint32_t shaderLoc, WGPUVertexFormat format = WGPUVertexFormat_Float32x2) 
            : mShaderLoc{shaderLoc}, mFormat{format}
        {
            switch(format) 
            {
                case WGPUVertexFormat_Float32x2: mSize = 2*4; break;
                case WGPUVertexFormat_Float32x3: mSize = 3*4; break;
                default: mSize = 2*4; break;
            }
        }
        ~BufferAttribute() {}

        inline const WGPUVertexFormat GetFormat() const { return mFormat; }
        inline const uint32_t GetSize() const { return mSize; }
        inline const uint32_t GetShaderLocation() const { return mShaderLoc; }

        uint32_t mOffset = 0; // The unique, per attribute offset is set by the buffer layout, since it needs information about all attributes.

    private:

        uint32_t mSize = 0;
        uint32_t mShaderLoc = 0;
        WGPUVertexFormat mFormat;
};

class BufferLayout 
{
    public:

        BufferLayout(std::vector<BufferAttribute>& attribs)
            : mAttribs{attribs} 
        {
            
        }

        ~BufferLayout() {}

        inline const uint32_t GetBufferStride() const { return mStride;}
        inline const std::vector<BufferAttribute>& GetAttributes() const { return mAttribs; }

        void CalculateStrideAndOffset() 
        {
            for(auto& attrib : mAttribs) 
            {
                attrib.mOffset = mStride;
                mStride += attrib.GetSize();
            }
        }
    
    private:

        std::vector<BufferAttribute>& mAttribs;
        uint32_t mStride = 0;
};

