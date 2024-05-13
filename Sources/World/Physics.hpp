#include "Systems.hpp"

class Physics : public IEntitySystem
{
public:
    Physics();
    ~Physics();

    void Run() override;
};