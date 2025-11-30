// tests/test_constructors.cpp

#include "utilities/test_helpers.hpp"

void test_vec2_constructor()
{
    std::cout << "\n--- TEST: Constructores Vec2 ---\n";
    vec2 v;
    print_vec2(v);
    std::cout << " <-- vec2 por defecto\n";
    vec2 v2(1.0f, 2.0f);
    print_vec2(v2);
    std::cout << " <-- vec2 parametrizado\n";
}

void test_body_constructor()
{
    std::cout << "\n--- TEST: Constructor Body ---\n";
    body b = create_body(0, 0, 1, 1, 1, 0.5f);
    print_vec2(b.posicion);
    std::cout << " | masa: " << b.masa << " | radio: " << b.radio << "\n";
}

void test_world_constructor()
{
    std::cout << "\n--- TEST: Constructor World ---\n";
    std::vector<body> cuerpos;
    cuerpos.push_back(create_body(0, 0, 0, 0, 1, 1));
    world w(cuerpos, vec2(0, -9.8f), 0.016f);
    std::cout << "World bodies: " << w.bodies.size() << "\n";
}

void test_world_constructors()
{
    test_vec2_constructor();
    test_body_constructor();
    test_world_constructor();
}