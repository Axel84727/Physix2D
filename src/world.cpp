#include "body.hpp"
#include "vec2.hpp"
#include "world.hpp"
// world(std::vector<body> &b, const vec2 &gravedad, float delta_time);
world::world() : bodies(), gravedad(), delta_time(0.0f) {}

world::world(const std::vector<body> &b_param, const vec2 &gravedad_param, float delta_time_param) : bodies(b_param), gravedad(gravedad_param), delta_time(delta_time_param) {}