#pragma once
#include <iostream>
#include <GLFW/glfw3.h>
#include "Systems.hpp"
#include "./World.hpp"
#include "../Program/Input.hpp"

class PlayerController : public IEntitySystem 
{
public:
    PlayerController() {}
    ~PlayerController() {}

    void Run() override 
    {
        for(auto& ent : World::GetEntities()) 
        {
            if(ent.flags & (Entiy_Flags::PLAYER_CONTROLLED | Entity_Flags::HAS_MOTION)) 
            {
                if(Input::IsKeyPressed(GLFW_KEY_W)) { ent.velocity.y += 1.0f; }
                if(Input::IsKeyPressed(GLFW_KEY_A)) { ent.velocity.x -= 1.0f; }
                if(Input::IsKeyPressed(GLFW_KEY_S)) { ent.velocity.y -= 1.0f; }
                if(Input::IsKeyPressed(GLFW_KEY_D)) { ent.velocity.x += 1.0f; }
            }
        }
    }
};