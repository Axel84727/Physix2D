#include "physics/body.hpp"

body::body(const vec2 &position_param,
           const vec2 &velocity_param,
           const vec2 &acceleration_param,
           float mass_param,
           float inv_mass_param,
           float radius_param,
           float restitution_param)
    // Lista de inicialización
    : position(position_param),
      previous_position(position_param),
      velocity(velocity_param),
      acceleration(acceleration_param),
      mass(mass_param),
      inv_mass(inv_mass_param),
      radius(radius_param),
      restitution(restitution_param)
{
    if (mass_param <= 0.0f)
    {
        inv_mass = 0.0f;
    }
}

body::body()
    : position(),
      previous_position(),
      velocity(),
      acceleration(),
      mass(0.0f),
      inv_mass(0.0f),
      radius(0.0f),
      restitution(1.0f)
{
}