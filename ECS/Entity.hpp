#pragma once
#include <glm/glm.hpp>
#include "../Geometry.hpp"

using EntityID = uint16_t;

struct Renderable 
{
    Geometry* geometry = nullptr;
    std::vector<UniformBufferObject> uniforms{};
};

typedef enum Entity_Flags 
{
    ACTIVE               = 1 << 0,
    PLAYER_CONTROLLED    = 1 << 1,
    HAS_MOTION           = 1 << 2,

} Entiy_Flags;


struct AABB_Bounds 
{
    uint32_t width;
    uint32_t height;
};

struct Transform 
{
    glm::vec3 position{0.0f};
    glm::vec3 scale{1.0f};
    glm::vec3 rotation{0.0f};
    glm::mat4 model_matrix{1.0f};
};

struct Entity 
{
    EntityID id;
    std::string label{"Default_Entity"};
    Renderable renderable;
    Transform world_transform;
    glm::vec3 velocity{0.0f};
    glm::vec3 acceleration{0.0f};
    AABB_Bounds bounds{50, 50};
    uint32_t flags;
    unsigned char* texture_data{nullptr};
    glm::vec3 color{1.0f, 0.5f, 0.0f};
};



