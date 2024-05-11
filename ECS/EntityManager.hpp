#include <stdlib.h>
#include <bitset>
#include <queue>
#define MAX_ENTITIES 5000
#define MAX_COMPONENTS 256

using EntityID = uint16_t;
using ComponentType = unsigned char;

using Signature = std::bitset<MAX_COMPONENTS>;

class EntityManager 
{ 
public:
    EntityManager();

    ~EntityManager();

    EntityID CreateEntity();

    void DestroyEntity(EntityID id);

    void SetSignature(EntityID id, Signature signature);

    Signature GetSignature(EntityID id);

private:
    std::queue<EntityID> mAvailableEntities{};

    std::array<Signature, MAX_COMPONENTS> mSignatures{};

    uint16_t mEntitiesCount{0};
};