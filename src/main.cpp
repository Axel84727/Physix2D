#include <iostream>
#include <vector>
// Solo necesitamos incluir world.hpp, ya que incluye body.hpp y vec2.hpp
#include "world.hpp"

// Función auxiliar para imprimir vec2
void print_vec2(const vec2 &v)
{
    std::cout << "(" << v.x << ", " << v.y << ")";
}

int main()
{
    std::cout << "--- PRUEBA DE CONSTRUCTORES ---" << std::endl;

    // 1. Pruebas de vec2

    // Constructor por Defecto: vec2() -> (0, 0)
    vec2 v_cero;
    std::cout << "1.1 Vec2 por Defecto: ";
    print_vec2(v_cero);
    std::cout << std::endl;

    // Constructor Parametrizado: vec2(float, float)
    vec2 v_pos_b1(10.0f, 5.0f);
    vec2 v_vel_b1(2.0f, 0.0f);
    vec2 v_accel_b1(0.0f, -9.8f);

    std::cout << "1.2 Vec2 Parametrizado: ";
    print_vec2(v_pos_b1);
    std::cout << std::endl;

    // 2. Pruebas de body

    // Masa e Inversa de Masa
    float masa_b1 = 50.0f;
    float inv_masa_b1 = 1.0f / masa_b1;

    // Constructor Parametrizado de body (usa const vec2& para las copias)
    body b1(v_pos_b1, v_vel_b1, v_accel_b1, masa_b1, inv_masa_b1, 1.0f);

    std::cout << "2. Body Inicializado:" << std::endl;
    std::cout << "   Posicion: ";
    print_vec2(b1.posicion);
    std::cout << ", Masa: " << b1.masa << ", Inv_M: " << b1.inv_mass << std::endl;

    // 3. Pruebas de world (Sobrecarga de Constructores)

    // a) Constructor por Defecto: world()
    world mundo_vacio;
    std::cout << "3.1 World por Defecto: Bodies = " << mundo_vacio.bodies.size();
    std::cout << ", Gravedad = ";
    print_vec2(mundo_vacio.gravedad);
    std::cout << std::endl;

    // b) Constructor Parametrizado: world(const std::vector<body>&, const vec2&, float)

    // Creamos el vector que pasaremos por referencia constante
    std::vector<body> cuerpos_iniciales;
    cuerpos_iniciales.push_back(b1);

    // Creamos un segundo cuerpo para hacer el vector más grande
    body b2(vec2(-10.0f, 0.0f), v_vel_b1, v_accel_b1, 5.0f, 1.0f / 5.0f, 0.3f);
    cuerpos_iniciales.push_back(b2);

    vec2 g_simulacion(0.0f, -10.0f);

    // Llama al constructor que copia el vector cuerpos_iniciales al miembro world::bodies
    world mi_mundo(cuerpos_iniciales, g_simulacion, 0.016f); // 0.016s es ~60 FPS

    std::cout << "3.2 World Parametrizado:" << std::endl;
    std::cout << "   Total Bodies: " << mi_mundo.bodies.size() << std::endl;
    std::cout << "   Gravedad: ";
    print_vec2(mi_mundo.gravedad);
    std::cout << ", Delta Time: " << mi_mundo.delta_time << std::endl;
    std::cout << "   Posicion del Body 1 copiado: ";
    print_vec2(mi_mundo.bodies[0].posicion);
    std::cout << std::endl;

    return 0;
}