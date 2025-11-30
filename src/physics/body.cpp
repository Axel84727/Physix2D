// body(const vec2 &posicion, const vec2 &velocidad, const vec2 &aceleracion, float masa, float inv_mass, float radio);
#include "physics/body.hpp"
body::body(const vec2 &posicion,
           const vec2 &velocidad,
           const vec2 &aceleracion,
           float masa,
           float inv_mass,
           float radio,
           float restitucion_param) : posicion(posicion), posicion_previa(posicion), velocidad(velocidad), aceleracion(aceleracion), masa(masa), inv_mass(inv_mass), radio(radio), restitucion(restitucion_param) {}

body::body() : posicion(), posicion_previa(), velocidad(), aceleracion(), masa(0.0f), inv_mass(0.0f), radio(0.0f), restitucion(1.0f) {}