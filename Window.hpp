#include <GLFW/glfw3.h>

class Window 
{
    public:

        Window(uint32_t w, uint32_t h, const char* label);
        ~Window();


        bool WindowShouldClose() const { return mWindowShouldClose; } 

    private:

        static void OnWindowCloseCallback(GLFWwindow* window);

        static bool mWindowShouldClose;
        GLFWwindow* mWindow = nullptr;

        uint32_t mWidth;
        uint32_t mHeight;

        const char* mLabel;
};