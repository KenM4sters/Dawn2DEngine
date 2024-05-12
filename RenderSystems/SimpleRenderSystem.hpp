#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "RenderSystem.hpp"
#include "../Pipeline.hpp"
#include "../Geometry.hpp"
#include "../ECS/Entity.hpp"





class SimpleRenderSystem : public RenderSystem 
{
    public:
        SimpleRenderSystem(const std::shared_ptr<Device>& device);
        
        ~SimpleRenderSystem() override;

        void UpdateBuffers() override;

        void Run(WGPURenderPassEncoder encoder) override;

    private:
        
        void SetDefaultRenderingData(Entity* ent);

        WGPUPipelineLayout mPipelineLayout;

        std::unique_ptr<Pipeline> mPipeline;

        std::vector<Entity>& mEntities;
};