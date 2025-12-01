cmake_minimum_required(VERSION 3.10)

# Define el proyecto principal y habilita C++
# Comentamos CUDA por ahora, lo descomentamos en el siguiente paso
project(CudaPlayground LANGUAGES CXX) 

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ----------------------------------------------------------------
# 1. CONFIGURACIÓN DE RAYLIB
# ----------------------------------------------------------------
find_package(raylib REQUIRED)

# ----------------------------------------------------------------
# 2. DEFINICIÓN DE ARCHIVOS FUENTE
# ----------------------------------------------------------------
set(SOURCE_FILES
    src/main.cpp
    src/math/vec2.cpp 
    src/physics/body.cpp 
    src/physics/world.cpp 
    src/sim/movementSystem.cpp 
    src/sim/collisionSystem.cpp
    src/sim/systemManager.cpp
)

# ----------------------------------------------------------------
# 3. CREACIÓN DEL EJECUTABLE
# ----------------------------------------------------------------
add_executable(CudaPlayground ${SOURCE_FILES})

# ----------------------------------------------------------------
# 4. INCLUSIÓN Y ENLACE DE LIBRERÍAS
# ----------------------------------------------------------------

# Directorios de Inclusión Públicos (Headers de la simulación)
target_include_directories(CudaPlayground PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

# Enlace de Librerías (Raylib, math, etc.) al ejecutable final
target_link_libraries(CudaPlayground PUBLIC 
    ${raylib_LIBRARIES} 
    m 
)

# Directorios de Inclusión de Raylib
target_include_directories(CudaPlayground PUBLIC ${raylib_INCLUDE_DIRS})


# ----------------------------------------------------------------
# 5. CONFIGURACIÓN DE PRUEBAS
# ----------------------------------------------------------------
add_subdirectory(tests)


# ----------------------------------------------------------------
# 6. CONFIGURACIÓN FUTURA DE CUDA (DESCOMENTAR EN LA PRÓXIMA FASE)
# ----------------------------------------------------------------
# # Habilita el compilador de CUDA (nvcc)
# project(CudaPlayground LANGUAGES CXX CUDA)

# # El estandar de C++ para CUDA 
# set(CMAKE_CUDA_STANDARD 17)
# set(CMAKE_CUDA_STANDARD_REQUIRED ON)