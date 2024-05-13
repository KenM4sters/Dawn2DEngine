#include <cassert>
#include <string.h>
#include <stack>
#include <algorithm>
#include "CollisionObserver.hpp"
#include "Entity.hpp"
#include "../Events/Event.hpp"


static glm::vec3 compass[] = 
{
    glm::vec3(0.0f, 1.0f, 0.0f),
    glm::vec3(1.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, -1.0f, 0.0f),
    glm::vec3(-1.0f, 0.0f, 0.0f)
};

CollisionObserver::CollisionObserver(uint32_t width, uint32_t height)
    : mGridWidth(width), mGridHeight(height)
{
    GenerateSpatialGrid();
}

void CollisionObserver::GenerateSpatialGrid() 
{
    const uint32_t wSegments = 2;
    const uint32_t hSegments = 2;

    for(int i = 0; i < hSegments; i++) 
    {
        for(int j = 0; j < wSegments; j++) 
        {
            GridCell cell{};
            cell.Position = glm::vec2(mGridWidth * (j/wSegments), mGridHeight * (i/hSegments));
            cell.Size = glm::vec2(mGridWidth / wSegments, mGridHeight / hSegments);

            mCells.push_back(cell);
        }
    }    
}

void CollisionObserver::Run()
{
    ClearAndAssignGrids();

    for(auto& entityArray : mSpatialGrid) 
    {
        auto& v = entityArray.second;
        for(int i = 0; i < v.size(); i++) 
        {
            for(int j = 0; j < v.size(); j++) 
            {
                if(i == j) continue;
                if(CheckCollision(v[i], v[j])) 
                {
                    // CollisionDirection dir = CalculateCollisionDirection(v[i], v[j]);

                    // switch(dir) 
                    // {
                    //     case CollisionDirection::ABOVE:  v[i]->velocity.y = std::max(v[i]->velocity.y, 0.0f); break;
                    //     case CollisionDirection::BELOW:  v[i]->velocity.y = std::min(v[i]->velocity.y, 0.0f); break;
                    //     case CollisionDirection::LEFT:   v[i]->velocity.x = std::min(v[i]->velocity.x, 0.0f); break;
                    //     case CollisionDirection::RIGHT:  v[i]->velocity.x = std::max(v[i]->velocity.x, 0.0f); break;
                    // }

                    mEventBus->Publish(new CollisionEvent(v[i], v[j]));
                } 
            }
        }
    }
}

CollisionDirection CollisionObserver::CalculateCollisionDirection(const Entity* ent1, const Entity* ent2) const 
{

    glm::vec3 entityDir = glm::normalize(ent2->world_transform.position - ent1->world_transform.position);


    // std::stack<float> closestAngle = std::stack<float>{};
    // closestAngle.push(360);
    // unsigned int collisionDir = 180;

    // for(int i = 0; i < 4; i++) 
    // {
    //     float angle = glm::dot(entityDir, compass[i]);
    //     if(angle < closestAngle.top()) 
    //     {
    //         closestAngle.push(angle);
    //         collisionDir = i;
    //         std::cout << angle << std::endl;
    //     }
    // }



    float max = 0.0f;
    unsigned int best_match = -1;
    for (unsigned int i = 0; i < 4; i++)
    {
        float dot_product = glm::dot(entityDir, compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
            std::cout << max << std::endl;
        }
    }

    return (CollisionDirection)best_match;
} 


void CollisionObserver::ClearAndAssignGrids() 
{
    mSpatialGrid.clear();

    for(auto& ent : World::GetEntities()) 
    {
        AssignGridToEntity(&ent);
    }
}

void CollisionObserver::AssignGridToEntity(Entity* ent) 
{
    const int cXcell = round((float)ent->world_transform.position.x / mGridWidth);
    const int cYcell = round((float)ent->world_transform.position.y / mGridHeight);

    const std::string cell = std::to_string(cXcell) + std::to_string(cYcell);

    AddEntityToCells(cell, ent);  
}

void CollisionObserver::AddEntityToCells(const std::string& cell, Entity* ent) 
{
    if(mSpatialGrid.count(cell)) 
    {
        mSpatialGrid[cell].push_back(ent);
    }
    else 
    {
        mSpatialGrid[cell] = std::vector<Entity*>{ent};
    }
}

bool CollisionObserver::CheckCollision(const Entity* ent1, const Entity* ent2) const 
{
    const int w1 = ent1->bounds.width;
    const int w2 = ent2->bounds.width;

    const int h1 = ent1->bounds.height;
    const int h2 = ent2->bounds.height;

    const glm::vec3 p1 = ent1->world_transform.position;
    const glm::vec3 p2 = ent2->world_transform.position;

    // collision x-axis?
    bool collisionX = p1.x + h1 >= p2.x &&
        p2.x + w2 >= p1.x;
    // collision y-axis?
    bool collisionY = p1.y + h1 >= p2.y &&
        p2.y + h2 >= p1.y;
    // collision only if on both axes
    return collisionX && collisionY;
}
