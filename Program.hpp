#include "Window.hpp"
#include <iostream>

class Program 
{
    public:
        Program(uint32_t w, uint32_t h, const char* label);
        ~Program();

        void Run() const;
    
    private:
        
        std::shared_ptr<Window> mWindow = nullptr;
};