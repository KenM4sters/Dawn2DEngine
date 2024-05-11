#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

struct Transform 
{
    glm::vec3 Position;
    glm::vec3 Scale;
    glm::quat Rotation;
    glm::mat4 ModelMatrix;
};

struct Collider 
{ 
    uint32_t Width;
    uint32_t Height;
};
 
struct Physics 
{
    glm::vec3 Velocity;
    glm::vec3 Acceleration;
};

struct Sprite 
{
    std::vector<float> Vertices;
    glm::vec3 Albedo;
}; 

struct Health 
{
    float HP;
};



 