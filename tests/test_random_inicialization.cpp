#include "utilities/test_helpers.hpp"
#include "sim/movementSystem.hpp"
#include "sim/collisionSystem.hpp"
#include "sim/systemManager.hpp"
#include <memory>
#include <iostream>

void test_basic_simulation_step()
{
    std::cout << "\n--- TEST: Basic Simulation Step (Random) ---\n";

    // create_random_world(number_of_bodies, gravity_vector, delta_time)
    world w = create_random_world(1, vec2(0, -9.8f), 0.016f);

    systemManager manager;

    manager.addSystem(std::make_unique<movementSystem>());
    manager.addSystem(std::make_unique<collisionSystem>());

    manager.update(w, w.delta_time);

    // Check the position of the first body after one step
    std::cout << "First body's position Y after 1 step: " << w.bodies[0].position.y << "\n";
}

void test_world_random_initialization()
{
    test_basic_simulation_step();
}