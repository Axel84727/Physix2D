# Benchmarking — Instrucciones para compilar y ejecutar

Este documento explica cómo configurar, compilar y ejecutar el runner de benchmarks headless que mide el tiempo de actualización de la simulación y cómo usar los scripts Python para analizar los resultados.

Contenido rápido

- Requisitos
- Compilar (sin CUDA)
- Ejecutar el benchmark (headless)
- Formato de salida (CSV)
- Analizar resultados con Python
- Notas sobre CUDA (para más adelante)

1. Requisitos

- Sistema: macOS / Linux (las instrucciones usan zsh/bash)
- CMake >= 3.10
- Compilador C++ con soporte C++17 (clang o gcc)
- Python 3 (para los scripts de análisis)
- Raylib (opcional): el ejecutable principal (`CudaPlayground`) enlaza Raylib; el target `benchmark` es headless y NO requiere Raylib.

2. Estructura relevante del repo

- `tools/benchmark.cpp` — runner headless que genera CSV en `benchmarks/`
- `include/utils/timer.hpp` — utilidades de temporización (Accumulator / ScopedTimer)
- `tools/bench_stats.py` — analiza un CSV y devuelve un JSON con estadísticas (mean/std)
- `tools/aggregate_benchmarks.py` — escanea `benchmarks/` y produce `benchmarks/summary.json`
- `CMakeLists.txt` — ahora contiene la opción `BUILD_BENCHMARK` para compilar (o no) el target `benchmark`.

3. Compilar (sin CUDA)

Desde la raíz del repositorio, ejecutar:

```bash
# Configurar (sin CUDA) y habilitar el benchmark
cmake -S . -B build -DENABLE_CUDA=OFF -DBUILD_BENCHMARK=ON

# Compilar (usa todos los núcleos disponibles)
cmake --build build --config Release -j$(sysctl -n hw.ncpu)
```

Notas:

- Si no tienes Raylib instalado y no quieres compilar el ejecutable gráfico, compila solo el target `benchmark`:

```bash
cmake --build build --config Release --target benchmark -j$(sysctl -n hw.ncpu)
```

4. Ejecutar el benchmark (headless)

El ejecutable `benchmark` se genera en `build/benchmark` (o `build/bin/benchmark`, dependiendo de la plataforma). Ejecuta:

```bash
./build/benchmark --n 1000 --frames 1000 --warmup 100
```

Opciones:

- `--n <N>`: número de cuerpos a crear (por defecto 1000)
- `--frames <M>`: número de frames medidos (por defecto 1000)
- `--warmup <W>`: frames de calentamiento antes de medir (por defecto 100)

Salida:

- El runner crea la carpeta `benchmarks/` (si no existe) y escribe un CSV con nombre `results-<timestamp>-N<N>.csv`.
- El CSV contiene las columnas: `frame,total_us,broad_us,narrow_us,resolve_us`. En la versión inicial `broad_us/narrow_us/resolve_us` pueden valer 0; `total_us` contiene el tiempo por frame en microsegundos.

5. Analizar resultados con Python

Analizar un CSV individual:

```bash
python3 tools/bench_stats.py benchmarks/results-2025xxxx-xxxxxx-N1000.csv
```

Esto imprime un JSON con estadísticas (frames, mean/std de `total`, `broad`, `narrow`, `resolve`).

Agregar/agrupar todos los CSV en `benchmarks/`:

```bash
python3 tools/aggregate_benchmarks.py
```

Esto produce `benchmarks/summary.json` con un objeto por cada CSV encontrado.

6. Interpretación rápida

- `total.mean` da la latencia promedio de actualización (µs).
- `broad.mean`, `narrow.mean`, `resolve.mean` (cuando estén disponibles) muestran cuánto contribuye cada fase. Con esas cifras podrás priorizar paralelización (por ejemplo, si `narrow` domina, paralelizar la comprobación de pares es una buena opción).

7. Notas sobre CUDA (para más adelante)

- El proyecto tiene la opción `ENABLE_CUDA` lista en `CMakeLists.txt`. Cuando quieras implementar la versión GPU:
  - Ejecuta `cmake -S . -B build -DENABLE_CUDA=ON` y compila.
  - Implementaremos kernels `.cu` y wrappers que realicen H2D/D2H explícitos para el integrador y/o la fase narrow.

8. Problemas frecuentes

- Permisos: si la carpeta `benchmarks/` no se puede crear por permisos, créala manualmente: `mkdir -p benchmarks && chmod 755 benchmarks`.
- Raylib ausente: compila solo `benchmark` con el target específico (ver arriba).

9. Ejemplo rápido (flujo completo)

```bash
# 1) Configurar y compilar
cmake -S . -B build -DENABLE_CUDA=OFF -DBUILD_BENCHMARK=ON
cmake --build build --config Release --target benchmark -j$(sysctl -n hw.ncpu)

# 2) Ejecutar benchmark
./build/benchmark --n 1000 --frames 1000 --warmup 100

# 3) Analizar
python3 tools/bench_stats.py benchmarks/results-<timestamp>-N1000.csv
python3 tools/aggregate_benchmarks.py
```
