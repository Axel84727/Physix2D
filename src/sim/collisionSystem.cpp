#include "sim/collisionSystem.hpp"
#include "physics/world.hpp"
#include <iostream>

// Provide constructor and destructor definitions so the linker can resolve them
collisionSystem::collisionSystem() {}
collisionSystem::~collisionSystem() {}

void collisionSystem::limpieza(world &world)
{
    for (auto &cell : world.grid)
    {
        cell.clear();
    }
}

void collisionSystem::repoblar(world &world)
{
    for (auto &body : world.bodies)
    {
        int tempIndex = world.get_grid_index(body.posicion);
        if (tempIndex >= 0)
        {
            world.grid[tempIndex].push_back(&body);
        }
    }
}
std::vector<std::pair<body *, body *>> collisionSystem::broad_phase(world &world)

{
    std::vector<std::pair<body *, body *>> pairs;

    for (size_t cellIndex = 0; cellIndex < world.grid.size(); ++cellIndex)
    {
        auto &currentCell = world.grid[cellIndex];
        int numCellsX = world.grid_info.num_cells_x;
        int numCellsY = world.grid_info.num_cells_y;

        int cy = cellIndex / numCellsX;
        int cx = cellIndex % numCellsX;

        int neighborOffset[3][2] = {
            {1, 0},
            {0, 1},
            {1, 1}};

        for (const auto offset : neighborOffset)
        {
            int dx = offset[0];
            int dy = offset[1];

            int neighborCx = cx + dx;
            int neighborCy = cy + dy;

            if (neighborCx >= numCellsX || neighborCy >= numCellsY)
            {
                continue;
            }

            int neighborIndex = neighborCy * numCellsX + neighborCx;
            auto &neighborCell = world.grid[neighborIndex];

            for (body *A : currentCell)
            {
                for (body *B : neighborCell)
                {
                    pairs.emplace_back(A, B);
                }
            }
        }

        for (size_t i = 0; i < currentCell.size(); ++i)
        {
            body *A = currentCell[i];
            for (size_t j = i + 1; j < currentCell.size(); ++j)
            {
                body *B = currentCell[j];
                pairs.emplace_back(A, B);
            }
        }
    }

    return pairs;
}
void collisionSystem::narrow_phase(world &world)

{
    auto pairs = broad_phase(world);
    for (auto &[A, B] : pairs)
    {
        if (check_collision(A, B, world))
        {
            resolve_collision(A, B, world);
        }
    }
}
/* Note: old resolve_collision implementation removed. See single canonical implementation below. */
void collisionSystem::resolve_collision(body *A, body *B, world &world)

{
    // Compute vector from A to B and its length
    vec2 diff = B->posicion - A->posicion;
    float dist2 = diff.x * diff.x + diff.y * diff.y;
    float distancia_neta = std::sqrt(dist2);
    if (distancia_neta <= 0.0f)
    {
        // Degenerate case: skip
        return;
    }

    float penetration = (A->radio + B->radio) - distancia_neta;
    if (penetration <= 0.0f)
        return; // no penetration

    // Normal pointing from A to B (unit)
    vec2 normal(diff.x / distancia_neta, diff.y / distancia_neta);

    // Inverse masses
    float invA = A->inv_mass;
    float invB = B->inv_mass;
    float invSum = invA + invB;
    if (invSum <= 0.0f)
        return; // both static

    // Positional correction (Baumgarte-like, but simple split by inverse mass)
    const float k_slop = 0.001f; // Reducido de 0.01f
    const float percent = 1.0f;  // Aumentado de 0.8f (Máxima corrección)
    float correctionMag = std::max(penetration - k_slop, 0.0f) / invSum * percent;
    vec2 correction = vec2(normal.x * correctionMag, normal.y * correctionMag);
    // Move A opposite normal, B along normal according to inverse mass
    A->posicion.x -= correction.x * invA;
    A->posicion.y -= correction.y * invA;
    B->posicion.x += correction.x * invB;
    B->posicion.y += correction.y * invB;

    // Relative velocity v_rel = vB - vA
    vec2 vRel = vec2(B->velocidad.x - A->velocidad.x, B->velocidad.y - A->velocidad.y);
    float velAlongNormal = dot(vRel, normal);

    // If velocities are separating, don't apply impulse
    if (velAlongNormal > 0.0f)
        return;

    // Restitution: use average of both bodies
    float e = (A->restitucion + B->restitucion) * 0.5f;

    // Impulse scalar
    float j = -(1.0f + e) * velAlongNormal;
    j /= invSum;

    // Apply impulse along the normal: impulse vector = j * normal
    vec2 impulse = vec2(j * normal.x, j * normal.y);

    // Debug logging for collisions involving a static body
    if (invA == 0.0f || invB == 0.0f)
    {
        std::cerr << "[debug] static collision\n";
        std::cerr << " A.pos=(" << A->posicion.x << "," << A->posicion.y << ") v=(" << A->velocidad.x << "," << A->velocidad.y << ") invA=" << invA << " mA=" << A->masa << " eA=" << A->restitucion << "\n";
        std::cerr << " B.pos=(" << B->posicion.x << "," << B->posicion.y << ") v=(" << B->velocidad.x << "," << B->velocidad.y << ") invB=" << invB << " mB=" << B->masa << " eB=" << B->restitucion << "\n";
        std::cerr << " normal=(" << normal.x << "," << normal.y << ") velAlongNormal=" << velAlongNormal << " j=" << j << "\n";
    }

    // Apply to velocities (A gets -impulse*invA, B gets +impulse*invB)
    A->velocidad.x -= impulse.x * invA;
    A->velocidad.y -= impulse.y * invA;

    B->velocidad.x += impulse.x * invB;
    B->velocidad.y += impulse.y * invB;

    // Clamp very small velocities to zero to avoid jitter on resting contacts
    const float velEpsilon = 1e-3f;
    if (std::fabs(A->velocidad.x) < velEpsilon)
        A->velocidad.x = 0.0f;
    if (std::fabs(A->velocidad.y) < velEpsilon)
        A->velocidad.y = 0.0f;
    if (std::fabs(B->velocidad.x) < velEpsilon)
        B->velocidad.x = 0.0f;
    if (std::fabs(B->velocidad.y) < velEpsilon)
        B->velocidad.y = 0.0f;
}

bool collisionSystem::check_collision(body *A, body *B, world &world)

{
    vec2 distancia = A->posicion - B->posicion;
    float distancia_al_cuadrado = (distancia.x * distancia.x + distancia.y * distancia.y);

    float suma_radios = A->radio + B->radio;

    float suma_radios_al_cuadrado = suma_radios * suma_radios;

    return distancia_al_cuadrado <= suma_radios_al_cuadrado;
}

void collisionSystem::update(world &world, float dt)
{

    limpieza(world);
    repoblar(world);
    narrow_phase(world);
}

void collisionSystem::solve_boundary_contacts(world &world)
{
    for (body &b : world.bodies)
    {
        if (b.inv_mass == 0.0f)
            continue;

        float ground_y = 0.0f;

        if (b.posicion.y - b.radio < ground_y)
        {
            // 1. REPOSICIÓN: Mover el cuerpo exactamente al límite (0.5)
            b.posicion.y = ground_y + b.radio;

            // 2. AMORTIGUACIÓN Y REBOTE
            if (b.velocidad.y < 0.0f)
            {
                // Si la velocidad es negativa (va hacia abajo)
                float restitucion_aplicada = b.restitucion;
                b.velocidad.y = -b.velocidad.y * restitucion_aplicada;
            }

            const float velEpsilon = 1e-3f;
            if (std::fabs(b.velocidad.y) < velEpsilon)
            {
                b.velocidad.y = 0.0f;
            }
        }
    }
}