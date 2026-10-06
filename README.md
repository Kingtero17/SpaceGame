# SpaceGame CLI - C++ Terminal Game

Un juego arcade de supervivencia espacial ejecutado íntegramente en la consola del sistema. En este título, el jugador pilota una nave con el objetivo de esquivar lluvias continuas de meteoritos. Desarrollado mediante un motor propio en C++ diseñado para operar en la terminal, destacando por su renderizado personalizado, detección de colisiones y menús en arte ASCII.

## Interfaces de Juego
<img width="926" height="421" alt="image" src="https://github.com/user-attachments/assets/5cc9aa38-3dc7-4e52-96fc-7d84e89ec71f" />


## Características Principales

*   **Generación Dinámica de Obstáculos:** Sistema de aparición de meteoritos para desafiar la capacidad de evasión y los reflejos del jugador en tiempo real.
*   **Motor de Renderizado Dinámico:** Actualización de frames en consola sin parpadeo (flickering), permitiendo el movimiento fluido de la nave y los meteoritos de forma simultánea.
*   **Gestión de Entradas (InputManager):** Captura de pulsaciones de teclado a bajo nivel para garantizar un control de evasión inmediato, asíncrono y sin bloqueos del hilo principal.
*   **Arquitectura Modular:** Separación estricta entre las mecánicas de la nave, el generador de meteoritos, el gestor de colisiones y el dibujo de interfaces (`Renderer`).

## Arquitectura y Estructura

```text
├── include/
│   ├── CollisionEngine.h   # Lógica y detección matemática de impactos[cite: 15]
│   ├── Game.h              # Bucle principal y control de estados del juego[cite: 15]
│   ├── InputManager.h      # Procesamiento asíncrono de eventos del teclado[cite: 15]
│   ├── MeteorManager.h     # Generación, físicas y comportamiento de meteoritos[cite: 15]
│   ├── Renderer.h          # Escritura, limpieza del búfer y dibujo en consola[cite: 15]
│   └── SpaceShipLogic.h    # Físicas, posicionamiento y mecánicas de la nave[cite: 15]
├── src/
│   ├── main.cpp            # Punto de entrada y orquestación del sistema[cite: 14]
└── README.md

## Requisitos Previos

*   Sistema Operativo: Windows (requerido para los binarios ejecutables `.exe` y la gestión nativa de la consola).
*   Compilador C++ (GCC/MinGW, Clang o MSVC).

## Compilación y Ejecución

Para compilar el código fuente en un archivo ejecutable, navega a la raíz del repositorio y ejecuta el compilador. Ejemplo utilizando `g++`:

```bash
# Compilar el código fuente
g++ src/*.cpp -I include -o SpaceGame.exe -O2

# Ejecutar el binario generado
./SpaceGame.exe
```

## Controles del Juego

**Movimiento de la Nave:**
*   `W` o `^` (Flecha Arriba) - Mover hacia arriba
*   `S` o `v` (Flecha Abajo) - Mover hacia abajo
*   `A` o `<` (Flecha Izquierda) - Mover a la izquierda
*   `D` o `>` (Flecha Derecha) - Mover a la derecha

**Acciones Generales:**
*   `1` - Jugar / Reiniciar partida (desde pausa)
*   `2` - Ver controles / Volver al menú de inicio
*   `3` - Pausar juego / Reanudar partida
*   `ESC` - Salir del juego
