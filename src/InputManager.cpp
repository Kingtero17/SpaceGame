#include "InputManager.h"

#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

static struct termios oldt, newt;
static int oldf;
#endif

void InputManager::init() {
#ifndef _WIN32
    // Configuracion para Linux: Activa modo "raw" no canonico y apaga el ECHO de los caracteres.
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    
    // Configurar file descriptors para que la funcion read() sea no bloqueante.
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
#endif
}

void InputManager::restore() {
#ifndef _WIN32
    // Restaura la terminal de Linux a su comportamiento estandar al salir del juego.
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
#endif
}

InputKey InputManager::getKeyPressed() {
    int ch = 0;
    bool pressed = false;

#ifdef _WIN32
    if (_kbhit()) {
        ch = _getch();
        // En Windows, las flechas y teclas especiales devuelven un codigo prefix (0 o 224/0xE0)
        // seguido del codigo real en la segunda llamada a _getch().
        if (ch == 0 || ch == 224) { 
            ch = _getch();
            switch (ch) {
                case 72: return InputKey::Up;
                case 80: return InputKey::Down;
                case 75: return InputKey::Left;
                case 77: return InputKey::Right;
                default: return InputKey::Any;
            }
        }
        pressed = true;
    }
#else
    char linux_ch = 0;
    if (read(STDIN_FILENO, &linux_ch, 1) > 0) {
        // En Linux, las teclas de flechas generan secuencias ANSI (ej. \033[A)
        if (linux_ch == '\033') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) > 0 && read(STDIN_FILENO, &seq[1], 1) > 0) {
                if (seq[0] == '[') {
                    switch (seq[1]) {
                        case 'A': return InputKey::Up;
                        case 'B': return InputKey::Down;
                        case 'C': return InputKey::Right;
                        case 'D': return InputKey::Left;
                        default: return InputKey::Any;
                    }
                }
            }
            return InputKey::Esc;
        }
        ch = linux_ch;
        pressed = true;
    }
#endif

    if (!pressed) {
        return InputKey::None;
    }

    switch (ch) {
        case 'w': case 'W': case '^': return InputKey::Up;
        case 's': case 'S': case 'v': return InputKey::Down;
        case 'a': case 'A': case '<': return InputKey::Left;
        case 'd': case 'D': case '>': return InputKey::Right;
        case 'p': case 'P': return InputKey::Pause;
        case 27:  return InputKey::Esc;
        case '1': return InputKey::Action1;
        case '2': return InputKey::Action2;
        case 13:  // Carriage Return
        case 10:  // Line Feed
            return InputKey::Enter;
        default:
            return InputKey::Any;
    }
}
