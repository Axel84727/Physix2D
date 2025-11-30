#pragma once
#include "vec2.hpp"

struct body
{
    vec2 posicion;
    vec2 posicion_previa;
    vec2 velocidad;
    vec2 aceleracion;
    float masa;
    float inv_mass;
    float radio;
    // basicamente uso const vec2& para agarrar el objeto en si y no usar una copia para hacerlo mas eficiente
    body();
    body(const vec2 &posicion, const vec2 &velocidad, const vec2 &aceleracion, float masa, float inv_mass, float radio);
};
