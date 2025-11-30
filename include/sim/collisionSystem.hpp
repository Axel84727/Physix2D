#pragma once
#include <vector>
#include <utility>
#include "ISystem.hpp"

class body;
class collisionSystem : public ISystem
{
private:
    void limpieza(world &world);
    void repoblar(world &world);
    std::vector<std::pair<body *, body *>> broad_phase(world &world);
    void narrow_phase(world &world);
    bool check_collision(body *A, body *B, world &world);
    void resolve_collision(body *A, body *B, world &world);
    void solve_boundary_contacts(world &world);

public:
    void update(world &, float dt) override;
    collisionSystem();
    ~collisionSystem();
};
