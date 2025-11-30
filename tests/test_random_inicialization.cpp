// tests/test_random_inicialization.cpp

#include "utilities/test_helpers.hpp"
#include "sim/movementSystem.hpp"
#include "sim/collisionSystem.hpp"
#include "sim/systemManager.hpp"
#include <memory>

void test_basic_simulation_step()
{
    std::cout << "\n--- TEST: Paso Básico de Simulación (Random) ---\n";

    world w = create_random_world(1, vec2(0, -9.8f), 0.016f);

    systemManager manager;

    manager.addSystem(std::make_unique<movementSystem>());
    manager.addSystem(std::make_unique<collisionSystem>());

    manager.update(w, w.delta_time);

    std::cout << "Posición Y del primer cuerpo después de 1 paso: " << w.bodies[0].posicion.y << "\n";
}

void test_world_random_initialization()
{
    test_basic_simulation_step();
}