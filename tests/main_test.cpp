// tests/main_test.cpp

#include <iostream>
// Declaraciones de funciones de prueba
void test_world_constructors();
void test_world_random_initialization();
void test_collision_elastic();
void test_collision_static();
void test_integrator_stability_wrapper(int type, const char *name); // Usamos un wrapper

int main()
{
    std::cout << "=============== INICIANDO PRUEBAS UNITARIAS ===============\n";

    test_world_constructors();
    test_world_random_initialization();

    test_collision_elastic();
    test_collision_static();

    std::cout << "\n--- TEST: Estabilidad de Integradores ---\n";
    test_integrator_stability_wrapper(0, "Euler Explícito");
    test_integrator_stability_wrapper(1, "Euler Semi-Implícito");
    test_integrator_stability_wrapper(2, "Verlet Position");

    std::cout << "================= PRUEBAS FINALIZADAS =================\n";
    return 0;
}