#include "sim/movementSystem.hpp"
#include "physics/body.hpp"
#include "physics/world.hpp"
// Provide constructor/destructor for movementSystem so linker can resolve symbols
movementSystem::movementSystem() {}
movementSystem::~movementSystem() {}
void movementSystem::integrate(world &world)
{
    for (body &b : world.bodies)
    {
        if (b.inv_mass <= 0)
        {
            continue;
        }

        vec2 aceleracion_total = b.aceleracion + world.gravedad;
        switch (world.integrador_actual)
        {
        case EULER_EXPLICIT:

            b.posicion = b.posicion + (b.velocidad * world.delta_time);
            b.velocidad = b.velocidad + (aceleracion_total * world.delta_time);
            break;

        case EULER_SEMI_IMPLICIT:
            b.velocidad = b.velocidad + (aceleracion_total * world.delta_time);

            b.posicion = b.posicion + (b.velocidad * world.delta_time);
            break;

        case VERLET_POSITION:
        {
            vec2 posicion_actual = b.posicion;

            vec2 delta_time2 = aceleracion_total * (world.delta_time * world.delta_time);
            vec2 siguiente_posicion = (b.posicion * 2.0f) - b.posicion_previa + delta_time2;

            b.posicion_previa = posicion_actual;
            b.posicion = siguiente_posicion;
            b.velocidad = (b.posicion - b.posicion_previa) * (1.0f / world.delta_time);
            break;
        }
        default:
            break;
        }
    };
}

void movementSystem::update(world &w, float dt)
{
    (void)dt;
    integrate(w);
}