#include "Systems.hpp"

class Physics : public ISystem
{
public:
    Physics();
    ~Physics();

    void Run() override;
};