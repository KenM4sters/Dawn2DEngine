#pragma once
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <utility>

class Window 
{
    public:
        Window(uint32_t w, uint32_t h, const char* label);

        ~Window();


        inline bool WindowShouldClose() const { return mWindowShouldClose; } 

        inline WGPUSurface GetWindowSurface(WGPUInstance instance) const { return glfwGetWGPUSurface(instance, mWindow); }

        inline GLFWwindow* GetNativeWindow() const { return mWindow; }

        const std::pair<int, int> GetWindowDimensions() const;

        const int GetWindowWidth() const; 
        
        const int GetWindowHeight() const; 

    private:
        static void OnWindowCloseCallback(GLFWwindow* window);

        static bool mWindowShouldClose;
        
        GLFWwindow* mWindow = nullptr;

        uint32_t mWidth;

        uint32_t mHeight;

        const char* mLabel;
};