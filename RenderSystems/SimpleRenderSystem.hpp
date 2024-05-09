#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderSystem.hpp"
#include "../Pipeline.hpp"
#include "../Geometry.hpp"




class SimpleRenderSystem : public RenderSystem 
{
    public:
        SimpleRenderSystem(const std::shared_ptr<Device>& device);
        
        ~SimpleRenderSystem() override;

        void UpdateBuffers() override;

        void Run(WGPURenderPassEncoder encoder) override;

    private:
        
        void CreatePipeline();

        WGPUPipelineLayout mPipelineLayout;

        std::unique_ptr<Pipeline> mPipeline;

        Geometry* mGeometry = nullptr;

        std::vector<UniformBufferObject> mUniformBuffers{};

        glm::mat4 mModel{1.0f};
        glm::vec3 mColor{1.0f, 0.5f, 0.0f};
};