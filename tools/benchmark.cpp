// Headless benchmark runner for the physics simulation.
// Produces CSV files with per-frame timings: frame,total_us,broad_us,narrow_us,resolve_us

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <sys/stat.h>

#include "physics/world.hpp"
#include "physics/body.hpp"
#include "sim/systemManager.hpp"
#include "sim/movementSystem.hpp"
#include "sim/collisionSystem.hpp"

// Minimal mkdir -p for portability
static void ensure_dir(const std::string &path)
{
    struct stat st;
    if (stat(path.c_str(), &st) != 0)
    {
        mkdir(path.c_str(), 0755);
    }
}

static std::string now_timestamp()
{
    std::time_t t = std::time(nullptr);
    char buf[64];
    std::strftime(buf, sizeof(buf), "%Y%m%d-%H%M%S", std::localtime(&t));
    return std::string(buf);
}

int main(int argc, char **argv)
{
    int N = 1000;
    int frames = 1000;
    int warmup = 100;
    for (int i = 1; i < argc; ++i)
    {
        std::string a = argv[i];
        if (a == "--n" && i + 1 < argc)
            N = std::stoi(argv[++i]);
        if (a == "--frames" && i + 1 < argc)
            frames = std::stoi(argv[++i]);
        if (a == "--warmup" && i + 1 < argc)
            warmup = std::stoi(argv[++i]);
    }

    ensure_dir("benchmarks");
    std::string ts = now_timestamp();
    std::string out_csv = "benchmarks/results-" + ts + "-N" + std::to_string(N) + ".csv";

    // Create world with N bodies in a grid
    std::vector<body> bodies;
    bodies.reserve(N);
    float spacing = 3.0f;
    int cols = std::max(1, (int)std::sqrt(N));
    for (int i = 0; i < N; ++i)
    {
        int x = i % cols;
        int y = i / cols;
        float px = (x - cols / 2) * spacing;
        float py = (y + 1) * spacing + 10.0f;
        bodies.push_back(body(vec2(px, py), vec2(0, 0), vec2(0, 0), 1.0f, 1.0f, 1.0f));
    }

    world sim_world(bodies, vec2(0.0f, -9.8f), 1.0f / 60.0f);

    // Prepare systems
    systemManager manager;
    manager.addSystem(std::make_unique<movementSystem>());
    manager.addSystem(std::make_unique<collisionSystem>());

    // Warmup
    for (int i = 0; i < warmup; ++i)
    {
        manager.update(sim_world, sim_world.delta_time);
    }

    // Measurement
    std::ofstream out(out_csv);
    out << "frame,total_us,broad_us,narrow_us,resolve_us\n";

    for (int f = 0; f < frames; ++f)
    {
        auto t0 = std::chrono::high_resolution_clock::now();
        manager.update(sim_world, sim_world.delta_time);
        auto t1 = std::chrono::high_resolution_clock::now();
        auto total_us = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();

        // For now, we don't have global accumulators; output zeros for sub-phases.
        out << f << "," << total_us << ",0,0,0\n";
    }

    out.close();
    std::cout << "Wrote " << out_csv << "\n";
    return 0;
}
