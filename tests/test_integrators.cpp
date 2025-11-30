#include "utilities/test_helpers.hpp"
#include "sim/movementSystem.hpp"
#include "sim/collisionSystem.hpp"
#include "sim/systemManager.hpp"
#include <iostream>
#include <memory>

void test_integrator_stability(IntegratorType type, const char *name)
{
    body A = create_body(0, 5, 0, -15, 1, 0.5f, 0.8f);
    body B = create_body(0, 0, 0, 0, 0, 1.0f, 0.8f);

    std::vector<body> bodies = {A, B};
    world w(bodies, vec2(0, -9.8f), 0.016f);
    w.integrador_actual = type;
    w.bodies[0].posicion_previa = w.bodies[0].posicion;

    systemManager manager;
    manager.addSystem(std::make_unique<collisionSystem>());
    manager.addSystem(std::make_unique<movementSystem>());
    for (int t = 0; t < 300; t++)
        manager.update(w, w.delta_time);

    std::cout << name << " final pos Y: " << w.bodies[0].posicion.y << "\n";
}

// Provide the wrapper with the signature expected by main_test.cpp
// main_test.cpp calls test_integrator_stability_wrapper(int, const char*)
// so we forward that to test_integrator_stability.
void test_integrator_stability_wrapper(int type, const char *name)
{
    // Map numeric type to IntegratorType and call the test helper
    IntegratorType itype = static_cast<IntegratorType>(type);
    test_integrator_stability(itype, name);
}