#pragma once
#include <iostream>
#include "./Entity.hpp"
#include "./Systems.hpp"
#include "../Graphics/Camera.hpp"

class World 
{
public:
    World(uint32_t width, uint32_t height);
    ~World();

    void Init(uint32_t width, uint32_t height);

    static inline std::vector<Entity>& GetEntities() { return mEntities; }

    void RunSystems() const;

    static inline std::shared_ptr<OrthographicCamera> GetCamera() { return mCamera; }

private:
    EntityID CreateEntity() const;
      
    void AddEntity(const Entity ent);

    std::vector<IEntitySystem*> mEntitySystems{};
    std::vector<IEventHandler*> mEventHandlers{};

    static std::vector<Entity> mEntities;
    static std::shared_ptr<OrthographicCamera> mCamera;

};