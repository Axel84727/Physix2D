#include "physics/world.hpp"
#include <cmath>
#include "physics/body.hpp"
#include "math/vec2.hpp"
// world(std::vector<body> &b, const vec2 &gravedad, float delta_time);
world::world() : bodies(), gravedad(), delta_time(0.0f) {}

world::world(const std::vector<body> &b_param, const vec2 &gravedad_param, float delta_time_param) : bodies(b_param), gravedad(gravedad_param), delta_time(delta_time_param)
{
    float width = grid_info.max_x - grid_info.min_x;
    float height = grid_info.max_y - grid_info.min_y;

    int numCellsX = static_cast<int>(std::ceil(width / grid_info.cell_size));
    int numCellsY = static_cast<int>(std::ceil(height / grid_info.cell_size));
    grid_info.num_cells_x = numCellsX;
    grid_info.num_cells_y = numCellsY;

    int totalCells = numCellsX * numCellsY;
    grid.resize(totalCells);
}

int world::get_grid_index(const vec2 &posicion) const
{
    int cx = static_cast<int>((posicion.x - grid_info.min_x) / grid_info.cell_size);
    int cy = static_cast<int>((posicion.y - grid_info.min_y) / grid_info.cell_size);

    if (cx < 0 || cx >= grid_info.num_cells_x ||
        cy < 0 || cy >= grid_info.num_cells_y)
    {
        return -1;
    }
    int index = cy * grid_info.num_cells_x + cx;

    return index;
}
