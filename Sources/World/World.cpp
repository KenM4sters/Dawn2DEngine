#include <webgpu/webgpu.hpp>
#include "World.hpp"
#include "PlayerController.hpp"
#include "CollisionObserver.hpp"
#include "CollisionHandler.hpp"
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

    for(const auto& system : mEntitySystems) 
    {
        delete system;
    }

    for(const auto& handler : mEventHandlers) 
    {
        delete handler;
    }
}

void World::Init(uint32_t width, uint32_t height) 
{

    // Systems
    //
    IEntitySystem* playerController = new PlayerController();
    IEntitySystem* collisionObserver = new CollisionObserver(width, height);
    IEntitySystem* physics = new Physics();
    mEntitySystems.push_back(playerController);
    mEntitySystems.push_back(collisionObserver);
    mEntitySystems.push_back(physics); // important to be pushed after collision observer.

    // Event Handlers.
    //
    IEventHandler* collisionHandler = new CollisionHandler();
    mEventHandlers.push_back(collisionHandler);
    
    // Orthographic Camera.
    //
    mCamera = std::make_shared<OrthographicCamera>(glm::vec3(0.0f, 0.0f, 0.5f), width, height);

    // Entities (bit messy - needs refactoring even if just slightly).
    //
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

    Transform boxTransform{};
    boxTransform.position = glm::vec3(400.0f, 325.0f, 0.0f);
    Entity box{};
    box.id = CreateEntity(); 
    box.color = glm::vec3(0.2f, 0.8f, 0.9f);
    box.flags = Entity_Flags::ACTIVE;
    box.texture_data = nullptr;
    box.bounds = {50, 50};
    box.world_transform = boxTransform;
    box.label = "Box";

    // AddEntity() must be called for each entity if it is to be made visible to any system.
    //
    AddEntity(enemy);
    AddEntity(box);
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
    for(const auto& system : mEntitySystems) 
    {
        system->Run();
    }  
}