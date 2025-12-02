#pragma once
#include <vector>
#include "math/vec2.hpp"
#include "physics/body.hpp"
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

    GridInfo grid_info;
    std::vector<float> position_x;
    std::vector<float> position_y;
    std::vector<float> previous_position_x;
    std::vector<float> previous_position_y;
    float gravity_x;
    float gravity_y;
    float delta_time;
    // Global damping coefficient (per-second). 0.0 means no global damping.
    // Interpreted as 1/s in the integrator and applied as exp(-d * dt).
    // small default damping to mimic air resistance / energy loss (1/s)
    float global_damping = 0.02f;
    // Per-frame collision phase timing accumulators (microseconds)
    unsigned long long broad_phase_us = 0;
    unsigned long long narrow_phase_us = 0;
    unsigned long long resolve_phase_us = 0;

    // The world is SoA-first and exposes SoA accessors for direct usage.

    std::vector<int> particle_cell_id;
    std::vector<int> particle_start_indices;
    std::vector<int> sorted_indices;
    // Grid: each cell holds a list of particle indices
    std::vector<std::vector<int>> grid;
    std::vector<float> vel_x;
    std::vector<float> vel_y;
    std::vector<float> acc_x;
    std::vector<float> acc_y;
    std::vector<float> mass;
    std::vector<float> inv_mass;
    std::vector<float> radius;
    std::vector<float> damping;
    std::vector<float> friction;
    std::vector<float> restitution;

    // Helpers
    size_t size() const { return position_x.size(); }
    void add_body(const body &b);
    void remove_body(size_t idx);
    vec2 get_position(size_t idx) const;
    void set_position(size_t idx, const vec2 &p);
    // Legacy conversion helpers removed: world is pure SoA now.
    // Provide SoA accessors for efficient upload to GPU or direct processing.
    const float *positions_x() const { return position_x.data(); }
    const float *positions_y() const { return position_y.data(); }
    const float *velocities_x() const { return vel_x.data(); }
    const float *velocities_y() const { return vel_y.data(); }
    const float *radii() const { return radius.data(); }
    size_t num_bodies() const { return size(); }

    // Property accessors (SoA-first)
    float get_restitution(size_t idx) const;
    float get_damping(size_t idx) const;
    float get_friction(size_t idx) const;

    // SoA constructor: accept pre-filled SoA vectors (move semantics).
    world(const std::vector<float> &position_x_in, const std::vector<float> &position_y_in, const vec2 &gravity_vec, float delta_time_in);

    world();

    world(
        std::vector<float> &&position_x_in,
        std::vector<float> &&position_y_in,
        float gravity_x,
        float gravity_y,
        float delta_time,

        // grid:
        std::vector<int> &&particle_cell_id_in,
        std::vector<int> &&particle_start_indices_in,
        std::vector<int> &&sorted_indices_in,

        // particles:
        std::vector<float> &&vel_x_in,
        std::vector<float> &&vel_y_in,
        std::vector<float> &&acc_x_in,
        std::vector<float> &&acc_y_in,
        std::vector<float> &&mass_in,
        std::vector<float> &&inv_mass_in,
        std::vector<float> &&radius_in);

    int get_grid_index(const vec2 &position) const;
};