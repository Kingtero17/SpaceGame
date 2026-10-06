#pragma once

enum class InputKey {
    None,
    Up,      // Flechas o 'w', 'W'
    Down,    // Flechas o 's', 'S'
    Left,    // Flechas o 'a', 'A'
    Right,   // Flechas o 'd', 'D'
    Pause,   // '3'
    Esc,     // ESC (27)
    Action1, // '1'
    Action2, // '2'
    Action3, // '3'
    Enter,   // ENTER o '\n'
    Any      // Cualquier otra tecla para salir del menu
};

class InputManager {
public:
    InputManager() = default;
    ~InputManager() = default;

    // Configura la terminal. En Linux ajusta termios a modo "raw" no canonico,
    // quitando el ECHO. En Windows puede no hacer nada si usamos _getch().
    void init();

    void restore();

    InputKey getKeyPressed();
};
