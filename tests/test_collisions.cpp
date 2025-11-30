// tests/test_collisions.cpp

#include "utilities/test_helpers.hpp"
#include "sim/collisionSystem.hpp"
#include <iostream>

void test_collision_elastic()
{
    std::cout << "\n--- TEST: Colisión Elástica (1D) ---\n";

    body A = create_body(-1, 0, 1, 0, 1, 1, 1.0f);
    body B = create_body(1, 0, -1, 0, 1, 1, 1.0f);

    std::vector<body> bodies = {A, B};

    world w(bodies, vec2(0, 0), 0.016f);

    collisionSystem cs;

    cs.update(w, 0.016f);

    std::cout << "Velocidad A X: " << w.bodies[0].velocidad.x << ", Velocidad B X: " << w.bodies[1].velocidad.x << "\n";
}

void test_collision_static()
{
    std::cout << "\n--- TEST: Colisión con Cuerpo Estático (Pared) ---\n";

    body A = create_body(-1, 0, 2, 0, 1, 1, 0.8f);
    body B = create_body(0, 0, 0, 0, 0, 1, 0.8f);

    std::vector<body> bodies = {A, B};
    world w(bodies, vec2(0, 0), 0.016f);

    collisionSystem cs;
    cs.update(w, 0.016f);

    std::cout << "Velocidad A X: " << w.bodies[0].velocidad.x << ", Velocidad B X: " << w.bodies[1].velocidad.x << "\n";
}