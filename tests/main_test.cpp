#include <iostream>
// Function declarations
void test_world_constructors();
void test_world_random_initialization();
void test_collision_elastic();
void test_collision_static();

int main()
{
    std::cout << "=============== STARTING UNIT TESTS ===============\n";

    test_world_constructors();
    test_world_random_initialization();

    test_collision_elastic();
    test_collision_static();

    // Removed specific integrator stability tests as only Verlet is used now.

    std::cout << "================= TESTS FINISHED =================\n";
    return 0;
}