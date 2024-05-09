#include <iostream>
#include <cassert>
#include "Device.hpp"

/**
 * Utility function to get a WebGPU device, so that
 *     WGPUAdapter device = requestDevice(adapter, options);
 * is roughly equivalent to
 *     const device = await adapter.requestDevice(descriptor);
 * It is very similar to requestAdapter
 */
WGPUDevice requestDevice(const WGPUAdapter adapter, WGPUDeviceDescriptor const * descriptor)
{
    struct UserData {
        WGPUDevice device = nullptr;
        bool requestEnded = false;
    };
    UserData userData;

    auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status, WGPUDevice device, char const * message, void * pUserData) {
        UserData& userData = *reinterpret_cast<UserData*>(pUserData);
        if (status == WGPURequestDeviceStatus_Success) {
            userData.device = device;
        } else {
            std::cout << "Could not get WebGPU device: " << message << std::endl;
        }
        userData.requestEnded = true;
    };

    wgpuAdapterRequestDevice(
        adapter,
        descriptor,
        onDeviceRequestEnded,
        (void*)&userData
    );

    assert(userData.requestEnded);

    return userData.device;
}



Device::Device(const WGPUAdapter adapter)
    : mAdapter(adapter) 
{
    std::cout << "Requesting device..." << std::endl;

    WGPUDeviceDescriptor deviceDesc = {};
    deviceDesc.nextInChain = nullptr;
    deviceDesc.label = "GPU Device"; // anything works here.
    deviceDesc.requiredFeaturesCount = 0; // we do not require any specific feature
    deviceDesc.requiredLimits = nullptr; // we do not require any specific limit
    deviceDesc.defaultQueue.nextInChain = nullptr;
    deviceDesc.defaultQueue.label = "The default queue"; // anything works here.

    mGPU = requestDevice(mAdapter, &deviceDesc);

    // Callback for capturing device related issues.
    auto onDeviceError = [](WGPUErrorType type, char const* message, void* /* pUserData */) 
    {
        std::cout << "Uncaptured device error: type " << type;
        if (message) std::cout << " (" << message << ")";
        std::cout << std::endl;
    };

    wgpuDeviceSetUncapturedErrorCallback(mGPU, onDeviceError, nullptr /* pUserData */);

    std::cout << "Device recieved: " << mGPU << std::endl;


    // Queue
    //
    mQueue = wgpuDeviceGetQueue(mGPU);

    auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status, void* /* pUserData */) 
    {
        std::cout << "Queued work finished with status: " << status << std::endl;
    };

    // Not sure what @param single value is here - doesn't seem to call though with it set to 0 or 1.
    wgpuQueueOnSubmittedWorkDone(mQueue, 0, onQueueWorkDone, nullptr);

    auto onDeviceLost = [](WGPUDeviceLostReason status, const char* message, void* /* pUserData */) 
    {
        std::cout << message << " with status:  " << status << std::endl;
    };

    wgpuDeviceSetDeviceLostCallback(mGPU, onDeviceLost, nullptr);
}

Device::~Device() 
{
    wgpuDeviceRelease(mGPU);
    wgpuAdapterRelease(mAdapter);
    wgpuQueueRelease(mQueue);
}



void Device::InitCommandEncoder() 
{
    WGPUCommandEncoderDescriptor encoderDesc = {};
    encoderDesc.nextInChain = nullptr;
    encoderDesc.label = "My command encoder";
    mEncoder = wgpuDeviceCreateCommandEncoder(mGPU, &encoderDesc);  
}

void Device::InitCommandBuffer() 
{
    WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
    cmdBufferDescriptor.nextInChain = nullptr;
    cmdBufferDescriptor.label = "Command buffer";
    mCommandBuffer = wgpuCommandEncoderFinish(mEncoder, &cmdBufferDescriptor);
}
