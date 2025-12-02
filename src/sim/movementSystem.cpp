#include "sim/movementSystem.hpp"
#include "physics/body.hpp"
#include "physics/world.hpp"
movementSystem::movementSystem() {}
movementSystem::~movementSystem() {}
void movementSystem::verlet_integration(world &simulation_world)
{
    // Pre-calculate time terms for efficiency and clarity
    const float delta_time = simulation_world.delta_time;
    const float delta_time_squared = delta_time * delta_time;
    const float inverse_delta_time = 1.0f / delta_time;

    // Iterate over all bodies using SoA arrays in world
    size_t n = simulation_world.position_x.size();
    for (size_t i = 0; i < n; ++i)
    {
        float inv_mass = simulation_world.inv_mass[i];
        if (inv_mass <= 0.0f)
            continue; // static

        // Start with global gravity
        vec2 total_acceleration(simulation_world.gravity_x, simulation_world.gravity_y);

        // Damping and friction currently not stored per-body in SoA; if legacy bodies exist, read them
        // Prefer SoA damping/friction if populated; this system reads SoA arrays.
        if (i < simulation_world.damping.size() && i < simulation_world.friction.size())
        {
            float d = simulation_world.damping[i];
            float f = simulation_world.friction[i];
            if (d != 0.0f)
            {
                vec2 vel(simulation_world.vel_x[i], simulation_world.vel_y[i]);
                total_acceleration = total_acceleration - (vel * d);
            }
            if (f != 0.0f)
            {
                vec2 vel(simulation_world.vel_x[i], simulation_world.vel_y[i]);
                float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
                if (speed > 1e-6f)
                {
                    vec2 vel_dir = vel * (1.0f / speed);
                    total_acceleration = total_acceleration - (vel_dir * (f * speed));
                }
            }
        }
        else
        {
            // Fallback: read via getters which check secondary storage if needed
            float d = simulation_world.get_damping(i);
            float f = simulation_world.get_friction(i);
            if (d != 0.0f)
            {
                vec2 vel(simulation_world.vel_x[i], simulation_world.vel_y[i]);
                total_acceleration = total_acceleration - (vel * d);
            }
            if (f != 0.0f)
            {
                vec2 vel(simulation_world.vel_x[i], simulation_world.vel_y[i]);
                float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);
                if (speed > 1e-6f)
                {
                    vec2 vel_dir = vel * (1.0f / speed);
                    total_acceleration = total_acceleration - (vel_dir * (f * speed));
                }
            }
        }

        // SoA access
        vec2 current_position(simulation_world.position_x[i], simulation_world.position_y[i]);
        vec2 previous_position(simulation_world.previous_position_x[i], simulation_world.previous_position_y[i]);

        vec2 acceleration_term = total_acceleration * delta_time_squared;
        vec2 next_position = (current_position * 2.0f) - previous_position + acceleration_term;

        // Update SoA arrays: shift current -> previous, write next
        simulation_world.previous_position_x[i] = simulation_world.position_x[i];
        simulation_world.previous_position_y[i] = simulation_world.position_y[i];
        simulation_world.position_x[i] = next_position.x;
        simulation_world.position_y[i] = next_position.y;

        // Update vel SoA using centered difference: (next - prev) / (2*dt)
        float half_inv_dt = 0.5f * inverse_delta_time;
        simulation_world.vel_x[i] = (simulation_world.position_x[i] - previous_position.x) * half_inv_dt;
        simulation_world.vel_y[i] = (simulation_world.position_y[i] - previous_position.y) * half_inv_dt;

        // Apply damping as exponential decay per second:
        // combined_damping (1/s) = world.global_damping + per-body damping
        float per_body_damping = 0.0f;
        if (i < simulation_world.damping.size())
            per_body_damping = simulation_world.damping[i];
        float combined_damping = simulation_world.global_damping + per_body_damping;
        if (combined_damping > 0.0f)
        {
            float damping_factor = std::exp(-combined_damping * delta_time);
            simulation_world.vel_x[i] *= damping_factor;
            simulation_world.vel_y[i] *= damping_factor;
            // Recompute previous_position to remain consistent with damped velocity
            if (simulation_world.delta_time > 0.0f)
            {
                simulation_world.previous_position_x[i] = simulation_world.position_x[i] - simulation_world.vel_x[i] * delta_time;
                simulation_world.previous_position_y[i] = simulation_world.position_y[i] - simulation_world.vel_y[i] * delta_time;
            }
        }

        // SoA arrays are the canonical storage.
    }
}

void movementSystem::update(world &simulation_world, float delta_time)
{
    verlet_integration(simulation_world);
}