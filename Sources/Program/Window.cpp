#include "Window.hpp"
#include <cassert>
#include <stdlib.h>
#include <iostream> 

bool Window::mWindowShouldClose = false;

Window::Window(uint32_t w, uint32_t h, const char* label) 
    : mWidth(w), mHeight(h), mLabel(label)
{
    glfwInit();

    if (!glfwInit()) 
    {
        std::cerr << "Could not initialize GLFW!" << std::endl;
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    mWindow = glfwCreateWindow(mWidth, mHeight, mLabel, NULL, NULL);

    if (!mWindow) 
    {
        std::cerr << "Could not open window!" << std::endl;
        glfwTerminate();
    }

    glfwSetWindowCloseCallback(mWindow, OnWindowCloseCallback);
    
}

Window::~Window() 
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

const std::pair<int, int> Window::GetWindowDimensions() const 
{
    return {mWidth, mHeight};
}   

const int Window::GetWindowWidth() const 
{
    auto[x, y] = GetWindowDimensions();
    return x;
} 

const int Window::GetWindowHeight() const 
{
    auto[x, y] = GetWindowDimensions();
    return y;
}

void Window::OnWindowCloseCallback(GLFWwindow* window) 
{
    mWindowShouldClose = true;
}


