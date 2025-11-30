#include "ISystem.hpp"

class world;
class movementSystem : public ISystem
{
private:
    /* data */
    void integrate(world &world);

public:
    void update(world &, float dt) override;
    movementSystem(/* args */);
    ~movementSystem();
};
