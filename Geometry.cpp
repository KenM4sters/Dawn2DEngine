#include "Geometry.hpp"

std::vector<float> SQUARE_VERTICES = 
{
    0.0, 1.0, // top-left
    1.0, 0.0, // bottom-right
    0.0, 0.0, // bottom-left 

    0.0, 1.0, // top-left
    1.0, 1.0, // top-right
    1.0, 0.0, // bottom-right
};

Geometry::Geometry(std::shared_ptr<Device>& device) 
    : mVertices{SQUARE_VERTICES}
{

    std::vector<BufferAttribute> attribs = 
    {
        BufferAttribute(0, WGPUVertexFormat_Float32x2), // Position attribute.
    };

    mBufferLayout = std::make_shared<BufferLayout>(attribs);

    CreateBuffers(device);
}

Geometry::~Geometry() 
{

}

void Geometry::CreateBuffers(std::shared_ptr<Device>& device) 
{
    // Create vertex buffer descriptor followed by the buffer itself.
    //
    WGPUBufferDescriptor bufferDesc{};
    bufferDesc.nextInChain = nullptr;
    bufferDesc.size = mVertices.size() * sizeof(float);
    bufferDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex;
    bufferDesc.mappedAtCreation = false;
    mVertexBuffer = wgpuDeviceCreateBuffer(device->GetDevice(), &bufferDesc);

    // Setting the native attributes array with our own custom BufferLayout class.
    //
    for(const auto& attrib : mBufferLayout->GetAttributes()) 
    {
        WGPUVertexAttribute nativeAttrib{};

        nativeAttrib.format = attrib.GetFormat();
        nativeAttrib.shaderLocation = attrib.GetShaderLocation();
        nativeAttrib.offset = attrib.mOffset;

        mNativeLayout.Attributes.push_back(nativeAttrib);
    }


    // Setting the native layout with the native attributes.
    //
    mNativeLayout.Layout.attributeCount = mNativeLayout.Attributes.size();
    mNativeLayout.Layout.attributes = mNativeLayout.Attributes.data();
    mNativeLayout.Layout.arrayStride = mBufferLayout->GetBufferStride();
    mNativeLayout.Layout.stepMode = WGPUVertexStepMode_Vertex;
}

