#include <webgpu/webgpu.hpp>
#include "World.hpp"
#include "PlayerController.hpp"
#include "CollisionObserver.hpp"
#include "CollisionSystem.hpp"
#include "Physics.hpp"

std::vector<Entity> World::mEntities = {};
std::shared_ptr<OrthographicCamera> World::mCamera = nullptr;


World::World(uint32_t width, uint32_t height) 
{
    Init(width, height);
}

World::~World() 
{
    for(const auto& ent : mEntities) 
    {
        delete ent.renderable.geometry;
    }
}

void World::Init(uint32_t width, uint32_t height) 
{

    // Systems
    //
    ISystem* playerController = new PlayerController();
    ISystem* collisionObserver = new CollisionObserver(width, height);
    ISystem* physics = new Physics();

    // Event Systems.
    //
    ISystem* collisionSystem = new CollisionSystem();

    mSystems.push_back(playerController);
    mSystems.push_back(collisionObserver);
    mSystems.push_back(physics); // important to be pushed after collision observer.
    
    mCamera = std::make_shared<OrthographicCamera>(glm::vec3(0.0f, 0.0f, 0.5f), width, height);

    Transform playerTransform{};
    playerTransform.position = glm::vec3(325.0f, 275.0f, 0.0f);
    Entity player{};
    player.id = CreateEntity(); 
    player.color = glm::vec3(1.0f, 0.5f, 0.0f);
    player.flags = Entity_Flags::ACTIVE | Entity_Flags::PLAYER_CONTROLLED;
    player.texture_data = nullptr;
    player.bounds = {50, 50};
    player.world_transform = playerTransform;
    player.label = "Player";

    Transform enemyTransform{};
    enemyTransform.position = glm::vec3(250.0f, 225.0f, 0.0f);
    Entity enemy{};
    enemy.id = CreateEntity(); 
    enemy.color = glm::vec3(0.5f, 0.1f, 0.8f);
    enemy.flags = Entity_Flags::ACTIVE;
    enemy.texture_data = nullptr;
    enemy.bounds = {50, 50};
    enemy.world_transform = enemyTransform;
    enemy.label = "Enemy";

    AddEntity(enemy);
    AddEntity(player);
}

void World::AddEntity(const Entity ent) 
{
    this->mEntities.push_back(ent);
}

EntityID World::CreateEntity() const
{
    static uint32_t counter = 0;
    return counter++;
}

void World::RunSystems() const 
{
    for(const auto& system : mSystems) 
    {
        system->Run();
    }  
}