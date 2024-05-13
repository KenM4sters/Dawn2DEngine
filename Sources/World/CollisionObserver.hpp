#pragma once
#include <unordered_map>
#include "Systems.hpp"
#include "./World.hpp"

struct GridCell 
{
    glm::vec2 Position{0.0f};
    glm::vec2 Size{1.0f};
};

enum CollisionDirection 
{
    ABOVE,
    BELOW,
    LEFT,
    RIGHT
};

class CollisionObserver : public ISystem 
{
public:
    CollisionObserver(uint32_t width, uint32_t height);

    ~CollisionObserver() {}

    void GenerateSpatialGrid();

    void Run() override;

private:
    void ClearAndAssignGrids();   

    void AssignGridToEntity(Entity* ent);

    void AddEntityToCells(const std::string& cell, Entity* ent);

    bool CheckCollision(const Entity* ent1, const Entity* ent2) const;

    CollisionDirection CalculateCollisionDirection(const Entity* ent1, const Entity* ent2) const; 

    std::unordered_map<std::string, std::vector<Entity*>> mSpatialGrid;

    std::vector<GridCell> mCells{};

    uint32_t mGridWidth = 0;
    uint32_t mGridHeight = 0;

};
