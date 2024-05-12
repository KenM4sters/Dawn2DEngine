#include "Input.hpp"
#include "Program.hpp"

bool Input::IsKeyPressed(const int keycode) 
{
    auto& program = Program::Get();
    bool state = glfwGetKey(program.GetWindow()->GetNativeWindow(), keycode);

    return state == GLFW_PRESS;
}