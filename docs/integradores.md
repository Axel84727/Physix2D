## Documentación: Cuándo Usar Cada Integrador

La elección del integrador numérico es crítica, ya que determina la **estabilidad** y la **conservación de energía** de la simulación de física. La decisión se basa en el compromiso entre simplicidad, velocidad de cálculo y robustez.

---

## 1. Integrador de Euler Explícito (**`EULER_EXPLICIT`**)

Este método utiliza las propiedades del tiempo actual ($t$) para calcular el estado futuro ($t+\Delta t$). Es el más simple de implementar.

- **Lógica:** La posición se actualiza usando la velocidad antigua ($\mathbf{v}_{\text{old}}$), lo que introduce el error de que el cambio de aceleración no se tiene en cuenta inmediatamente.
- **Fórmula Clave:** $\mathbf{x}_{t+1} = \mathbf{x}_t + \mathbf{v}_t \cdot \Delta t$
- **Estabilidad:** **Baja (Inestable).** La energía total del sistema tiende a **aumentar** con el tiempo, especialmente si el paso de tiempo ($\Delta t$) es grande o si hay fuerzas fuertes (como las que ocurren durante una colisión). Esto provoca que las simulaciones se "inflen" o **exploten** (valores $\mathbf{x} \to \infty$).
- **Cuándo Usarlo:**
  - **NO RECOMENDADO** para la simulación de cuerpos rígidos o colisiones.
  - Solo se puede usar de forma segura si el paso de tiempo ($\Delta t$) es **extremadamente pequeño** y las fuerzas no son excesivas. Su uso principal es didáctico o como base para métodos más avanzados.

---

## 2. Integrador de Euler Semi-Implícito (**`EULER_SEMI_IMPLICIT`**)

También conocido como _Euler-Cromer_, este método es una ligera reordenación del Euler Explícito que ofrece una estabilidad dramáticamente mejorada.

- **Lógica:** La velocidad se calcula primero con la aceleración actual ($\mathbf{v}_{\text{new}}$), y luego esa **nueva velocidad** se usa para calcular la nueva posición. Este orden introduce una pequeña amortiguación artificial, lo que mejora la estabilidad.
- **Fórmula Clave:** $\mathbf{v}_{t+1} = \mathbf{v}_t + \mathbf{a}_t \cdot \Delta t$ y luego $\mathbf{x}_{t+1} = \mathbf{x}_t + \mathbf{v}_{t+1} \cdot \Delta t$
- **Estabilidad:** **Alta (Estable).** El método tiende a **disipar** (perder) energía muy lentamente con el tiempo, lo que se manifiesta como una pequeña amortiguación. Esto es generalmente deseable en los juegos, ya que previene las explosiones y ayuda a estabilizar sistemas.
- **Cuándo Usarlo:**
  - **ESTÁNDAR DE LA INDUSTRIA (Juegos):** Es el integrador preferido en la mayoría de los motores de física de tiempo real debido a su excelente equilibrio entre simplicidad de implementación, velocidad y estabilidad robusta.

---

## 3. Integrador de Verlet por Posición (**`VERLET_POSITION`**)

El método de Verlet se distingue por no rastrear la velocidad directamente, sino que se basa en el historial de posiciones ($\mathbf{x}_t$ y $\mathbf{x}_{t-1}$).

- **Lógica:** Calcula la nueva posición ($\mathbf{x}_{t+1}$) a partir de las dos posiciones anteriores y la aceleración. La velocidad ($\mathbf{v}$) se infiere solo cuando es necesaria (por ejemplo, para la detección y resolución de colisiones).
- **Fórmula Clave:** $\mathbf{x}_{t+1} = 2\mathbf{x}_t - \mathbf{x}_{t-1} + \mathbf{a}_t \cdot \Delta t^2$
- **Estabilidad:** **Excelente (Conservador de Energía).** El método es simétrico en el tiempo, lo que significa que la **energía total** del sistema se mantiene prácticamente constante. No disipa ni amplifica la energía.
- **Cuándo Usarlo:**
  - **Simulaciones de Precisión:** Es ideal para sistemas donde la conservación de la energía a largo plazo es crítica, como **dinámica molecular, simulación de resortes, cuerdas o trayectorias orbitales**. Es robusto, pero requiere almacenar un estado extra (`posicion_previa`).
