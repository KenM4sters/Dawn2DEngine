#include "Physics.hpp"
#include "../World.hpp"

Physics::Physics() 
{

}

Physics::~Physics() 
{

}

void Physics::Run()
{
    for(auto& ent : World::GetEntities()) 
    {
        ent.velocity += ent.acceleration;
        ent.world_transform.position += ent.velocity;
    }
}
