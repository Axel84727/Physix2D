# CMakeLists.txt for tests

cmake_minimum_required(VERSION 3.15)
project(PhysicsTests)

# Include directory for headers
include_directories("../include")

# Source files for the main simulation engine
set(CORE_SRC_FILES
    ../src/math/vec2.cpp
    ../src/physics/body.cpp
    ../src/physics/world.cpp
    ../src/sim/collisionSystem.cpp
    ../src/sim/movementSystem.cpp
    ../src/sim/systemManager.cpp
)

# Source files for the tests themselves (uses GLOB to find all .cpp in this directory)
file(GLOB TEST_SRC_FILES *.cpp)

# Create the test executable
add_executable(run_tests ${CORE_SRC_FILES} ${TEST_SRC_FILES})

# Link against the necessary libraries if any (e.g., standard math library for sqrt/sin)
# target_link_libraries(run_tests m) # Uncomment if linking to math library is needed

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)