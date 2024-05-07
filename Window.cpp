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

void Window::OnWindowCloseCallback(GLFWwindow* window) 
{
    mWindowShouldClose = true;
}


