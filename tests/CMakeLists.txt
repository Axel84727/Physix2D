cmake_minimum_required(VERSION 3.15)
project(PhysicsTests)

# Carpeta include para headers
include_directories("../include")

# Fuentes principales
set(SRC_FILES
    ../src/math/vec2.cpp
    ../src/physics/body.cpp
    ../src/physics/world.cpp
    ../src/sim/collisionSystem.cpp
    ../src/sim/movementSystem.cpp
    ../src/sim/systemManager.cpp
)

# Fuentes de tests
file(GLOB TEST_FILES *.cpp)

# Ejecutable de tests
add_executable(run_tests ${SRC_FILES} ${TEST_FILES})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
