#pragma once
#include <vector>
#include "body.hpp"
#include "math/vec2.hpp"

enum IntegratorType
{
    EULER_EXPLICIT,
    EULER_SEMI_IMPLICIT,
    VERLET_POSITION
};
struct GridInfo
{
    float min_x = -100.0f;
    float max_x = 100.0f;
    float max_y = 100.0f;
    float min_y = -100.0f;

    const float cell_size = 5.0f;

    int num_cells_x = 0;
    int num_cells_y = 0;
};
struct world
{
    IntegratorType integrador_actual = EULER_SEMI_IMPLICIT;
    GridInfo grid_info;
    std::vector<body> bodies;
    vec2 gravedad;
    float delta_time;

    std::vector<std::vector<body *>> grid;
    world();
    world(const std::vector<body> &b, const vec2 &gravedad, float delta_time);
    int get_grid_index(const vec2 &posicion) const;
};
