#include "utilities/test_helpers.hpp"
#include "sim/collisionSystem.hpp"
#include <iostream>

// tests/test_collisions.cpp

void test_collision_elastic()
{
    std::cout << "\n--- TEST: Elastic Collision (1D) ---\n";

    // Body A: Pos X=-1.0, Vel X=1.0. Radius 1.0.
    body A = create_body(-1.0f, 0, 1, 0, 1, 1.0f, 1.0f);

    // Body B: Pos X=0.9, Vel X=-1.0. Radius 1.0. (Distance = 1.9, so they overlap by 0.1)
    body B = create_body(0.9f, 0, -1, 0, 1, 1.0f, 1.0f);

    std::vector<body> bodies = {A, B};

    // Use a small time step (0.016f)
    world w;
    w.gravity_x = 0.0f;
    w.gravity_y = 0.0f;
    w.delta_time = 0.016f;
    for (auto &b : bodies)
        w.add_body(b);

    collisionSystem cs;

    // The collision impulse should swap the velocities.
    // Expected Result: A.velocity.x = -1, B.velocity.x = 1.
    cs.update(w, w.delta_time);

    std::cout << "Velocity A X: " << w.vel_x[0] << ", Velocity B X: " << w.vel_x[1] << "\n";
}

void test_collision_static()
{
    std::cout << "\n--- TEST: Collision with Static Body (Wall) ---\n";

    // A: moving (mass=1), B: static (inv_mass=0)
    body A = create_body(-1, 0, 2, 0, 1, 1, 0.8f);
    body B = create_body(0, 0, 0, 0, 0, 1, 0.8f);

    std::vector<body> bodies = {A, B};
    world w;
    w.gravity_x = 0.0f;
    w.gravity_y = 0.0f;
    w.delta_time = 0.016f;
    for (auto &b : bodies)
        w.add_body(b);

    collisionSystem cs;
    cs.update(w, 0.016f);

    std::cout << "Velocity A X: " << w.vel_x[0] << ", Velocity B X: " << w.vel_x[1] << "\n";
}