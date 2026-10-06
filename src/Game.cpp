#include "Game.h"
#include <chrono>
#include <thread>

// Colores ANSI
#define COLOR_ROJO     "\x1b[31m"
#define COLOR_VERDE    "\x1b[32m"
#define COLOR_AMARILLO "\x1b[33m"
#define CP_AZUL        "\x1b[34m"
#define CP_CIAN        "\x1b[36m"
#define COLOR_RESET    "\x1b[0m"

Game::Game() 
    : currentState_(GameState::MAIN_MENU),
      currentInput_(InputKey::None),
      isRunning_(true),
      explosionFrameTimer_(0)
{
    inputManager_.init();
}

Game::~Game() {
    inputManager_.restore();
    renderer_.restore();
}

void Game::run() {
    // Ajustado para estabilizar el juego a ~14 FPS (70ms por frame)
    const int frameDelayMs = 70; 

    while (isRunning_) {
        auto frameStart = std::chrono::steady_clock::now();

        handleInput();
        update();
        render();

        auto frameEnd = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart).count();

        if (elapsedTime < frameDelayMs) {
            std::this_thread::sleep_for(std::chrono::milliseconds(frameDelayMs - elapsedTime));
        }
    }

    renderer_.clearBuffer();
    drawBorder();
    drawFinalScreen();
    renderer_.render();
    InputKey exitKey;
    do {
        exitKey = inputManager_.getKeyPressed();
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    } while (exitKey != InputKey::Enter && exitKey != InputKey::Esc);
}

void Game::handleInput() {
    currentInput_ = inputManager_.getKeyPressed();
}

void Game::update() {
    switch (currentState_) {
        case GameState::MAIN_MENU: updateMainMenu(); break;
        case GameState::CONTROLS_MENU: updateControlsMenu(); break;
        case GameState::PLAYING: updatePlaying(); break;
        case GameState::PAUSED: updatePaused(); break;
        case GameState::EXPLODING: updateExploding(); break;
        case GameState::GAME_OVER: updateGameOver(); break;
        case GameState::EXIT: isRunning_ = false; break;
    }
}

void Game::updateMainMenu() {
    if (currentInput_ == InputKey::Action1) {
        ship_.reset();
        meteors_.reset();
        currentState_ = GameState::PLAYING;
    } else if (currentInput_ == InputKey::Action2) {
        currentState_ = GameState::CONTROLS_MENU;
    } else if (currentInput_ == InputKey::Esc) {
        currentState_ = GameState::EXIT;
    }
}

void Game::updateControlsMenu() {
    if (currentInput_ != InputKey::None) {
        currentState_ = GameState::MAIN_MENU;
    }
}

void Game::updatePlaying() {
    if (currentInput_ == InputKey::Pause) {
        currentState_ = GameState::PAUSED;
        return;
    }

    if (currentInput_ == InputKey::Left) ship_.moveLeft();
    else if (currentInput_ == InputKey::Right) ship_.moveRight();

    if (meteors_.update()) {
        ship_.moveUp();
    }

    if (collisionEngine_.checkCollision(ship_, meteors_)) {
        ship_.loseHeart();
        currentState_ = GameState::EXPLODING;
        explosionFrameTimer_ = 0; 
    }
}

void Game::updatePaused() {
    if (currentInput_ == InputKey::Action1) {
        ship_.reset(); meteors_.reset(); currentState_ = GameState::PLAYING;
    } else if (currentInput_ == InputKey::Action2) {
        currentState_ = GameState::MAIN_MENU;
    } else if (currentInput_ == InputKey::Action3 || currentInput_ == InputKey::Pause) {
        currentState_ = GameState::PLAYING;
    } else if (currentInput_ == InputKey::Esc) {
        currentState_ = GameState::EXIT;
    }
}

void Game::updateExploding() {
    explosionFrameTimer_++;
    // A ~14fps, 5 frames = ~350ms (Similar al Sleep original)
    if (explosionFrameTimer_ > 5) {
        if (ship_.getHearts() == 0) {
            ship_.loseLife();
            ship_.resetHearts();
            if (ship_.getLives() == 0) {
                currentState_ = GameState::GAME_OVER;
            } else {
                currentState_ = GameState::PLAYING;
            }
        } else {
            currentState_ = GameState::PLAYING;
        }
    }
}

void Game::updateGameOver() {
    if (currentInput_ == InputKey::Action1) {
        ship_.reset(); meteors_.reset(); currentState_ = GameState::PLAYING;
    } else if (currentInput_ == InputKey::Action2) {
        currentState_ = GameState::MAIN_MENU;
    } else if (currentInput_ == InputKey::Esc) {
        currentState_ = GameState::EXIT;
    }
}

void Game::render() {
    renderer_.clearBuffer();

    switch (currentState_) {
        case GameState::MAIN_MENU:
            drawBorder(); drawMainMenu(); break;
        case GameState::CONTROLS_MENU:
            drawBorder(); drawControlsMenu(); break;
        case GameState::PLAYING:
        case GameState::PAUSED:
        case GameState::EXPLODING:
            drawBorder(); drawHUD();
            meteors_.draw(renderer_);
            if (currentState_ == GameState::EXPLODING) {
                int stage = (explosionFrameTimer_ < 2) ? 1 : 2;
                ship_.draw(renderer_, stage);
            } else {
                ship_.draw(renderer_, 0);
            }
            if (currentState_ == GameState::PAUSED) drawPauseMenu();
            break;
        case GameState::GAME_OVER:
            drawBorder(); drawGameOverMenu(); break;
        case GameState::EXIT: break;
    }

    renderer_.render();
}

void Game::drawBorder() {
    for (int i = 2; i < 118; ++i) {
        renderer_.drawChar(i, 2, static_cast<char>(205)); // ═
        renderer_.drawChar(i, 28, static_cast<char>(205));
    }
    for (int i = 2; i < 29; ++i) {
        renderer_.drawChar(2, i, static_cast<char>(186)); // ║
        renderer_.drawChar(117, i, static_cast<char>(186));
    }
    renderer_.drawChar(2, 2, static_cast<char>(201));   // ╔
    renderer_.drawChar(2, 28, static_cast<char>(200));  // ╚
    renderer_.drawChar(117, 2, static_cast<char>(187)); // ╗
    renderer_.drawChar(117, 28, static_cast<char>(188));// ╝
}

void Game::drawHUD() {
    renderer_.drawString(2, 1, "VIDAS " + std::to_string(ship_.getLives()));
    renderer_.drawString(55, 1, "NIVEL " + std::to_string(meteors_.getLevel()));
    for (int i = 0; i < ship_.getHearts(); ++i) {
        renderer_.drawChar(110 + i, 1, static_cast<char>(3));
    }
}

void Game::drawMainMenu() {
    static const char Portada [22][110] = {
	"    *        *          *             *    *            *           *      *      *        *    *          * ",
	"       *           *         *     *               *            *             *      *       *       *       ",
	" *          ******  ******  ******  ******  ******        *         *   *        *       *       *      *  * ",
	"    *   *        *  *    *  *    *  *       *          *          *        *         *       *       *       ",
	"            ******  ******  ******  *       ******  *        *         *         *       *              *    ",
	"  *   *     *       *       *    *  *       *            *      *         *   *      *       *    *        * ",
	"            ******  *       *    *  ******  ******         *        *                                *       ",
	"     *                    *    *          *        *                     -PRESIONE 1 PARA JUGAR.            *",
	"  *     *  |     *    *                                     *    *       -PRESIONE 2 PARA VER CONTROLES.  *  ",
	"     *     |  *           *    ee    *   *       *     |                 -PRESIONE ESC PARA SALIR.           ",
	"         | |      *     *     eeee           *         | |   *           *        *             *    *   *  *",
	"  *  *   | |              *   eeee      *              | |         *          *          *                   ",
	"         |    1111                            1111       | *    *                   *        *      *    *   ",
	"    *         1111    xxxx    xxxx    xxxx    1111   *   |               *                                 * ",
	" *         *  1111    xxxx    xxxx    xxxx    1111              *    *          *        *       *     *     ",
	"       aa      aa                              aa      aa               *           *         *            * ",
	"   *  aaaa    aaaa        aaaaaaaaaaaa   *    aaaa    aaaa *    *   *       *           *            *       ",
	" *    aaaa    aaaa   *    aaaaaaaaaaaa        aaaa    aaaa                    *     *          *           * ",
	"       aa      aa             aaaa        *    aa      aa     *    *      *                  *     *     *   ",
	"   *               *                               *                            *     *    *                 ",
	" *     *    *           *       *      *     *       *   *    *    *   *    *      *           *      *     *",
	};
    
    for(int i = 0; i < 21; i++){
        renderer_.drawString(5, i + 5, Portada[i]);
    }
}

void Game::drawControlsMenu() {
    int a = 33;
    renderer_.drawString(a, 6, "  ____ _____   _  ___ ______ _____    __    ____ ____ ", COLOR_AMARILLO);
    renderer_.drawString(a, 7, " / __// __  \\ / \\/  //_  __/ / __/   / /   / __// __/ ", COLOR_AMARILLO);
    renderer_.drawString(a, 8, "/ /_ / /_ / // \\/  /  / / / / / o \\ / /__ / _/_ \\ \\  ", COLOR_AMARILLO);
    renderer_.drawString(a, 9, "\\___/\\_____//_/ \\_/  /_/ /_/\\_\\___//____/ \\___//___/ ", COLOR_AMARILLO);

    int b = 45;
    renderer_.drawString(b, 12, "ARRIBA:    [ W ]  o  [ ^ ]");
    renderer_.drawString(b, 13, "ABAJO:     [ S ]  o  [ v ]");
    renderer_.drawString(b, 14, "IZQUIERDA: [ A ]  o  [ < ]");
    renderer_.drawString(b, 15, "DERECHA:   [ D ]  o  [ > ]");

    renderer_.drawString(b, 17, "PAUSA:     [ P ]", COLOR_ROJO);
    renderer_.drawString(b, 18, "SALIR:     [ ESC ]", COLOR_ROJO);

    renderer_.drawString(38, 22, "PRESIONE CUALQUIER TECLA PARA VOLVER AL MENU...");
}

void Game::drawPauseMenu() {
    renderer_.drawString(56, 7, "PAUSE");
    renderer_.drawString(28, 9, "SALIR:              Presione ESC para salir.");
    renderer_.drawString(28, 10, "REINICIAR PARTIDA:  Presione 1 para reiniciar la partida.");
    renderer_.drawString(28, 11, "MENU DE INICIO:     Presione 2 para volver al menu de inicio.");
    renderer_.drawString(28, 12, "REANUDAR PARTIDA:   Presione P para reanudar la partida.");
}

void Game::drawGameOverMenu() {
    int c = 32; 
    renderer_.drawString(c, 7, " _________    __  _________   ____ _   ____________ ", COLOR_ROJO);
    renderer_.drawString(c, 8, "/ ____/   |  /  |/  / ____/  / __ \\ | / / ____/ __ \\", COLOR_ROJO);
    renderer_.drawString(c, 9, "/ / __/ /| | / /|_/ / __/    / / / / |/ / __/ / /_/ /", COLOR_ROJO);
    renderer_.drawString(c, 10, "/ /_/ / ___ |/ /  / / /___   / /_/ /|  / /___/ _, _/ ", COLOR_ROJO);
    renderer_.drawString(c, 11, "\\____/_/  |_/_/  /_/_____/   \\____/ |_/_____/_/ |_|  ", COLOR_ROJO);

    int a = 28;
    renderer_.drawString(a, 13, "SALIR:              Presione ESC para salir.");
    renderer_.drawString(a, 14, "REINICIAR PARTIDA:  Presione 1 para reiniciar la partida.");
    renderer_.drawString(a, 15, "MENU DE INICIO:     Presione 2 para volver al menu de inicio.");
}

void Game::drawFinalScreen() {
    int x_centro = 30; 
    int y_ini = 8;

    renderer_.drawString(x_centro, y_ini++, "  _____ _   _    _   _   _ _  __ ____    _____ ___  ____  ", COLOR_VERDE);
    renderer_.drawString(x_centro, y_ini++, " |_   _| | | |  / \\ | \\ |  ||/ // ___|  |  ___/ _ \\|  _ \\ ", COLOR_VERDE);
    renderer_.drawString(x_centro, y_ini++, "   | | | |_| | / _ \\|  \\|  |' / \\___ \\  | |_ | | | | |_) |", COLOR_VERDE);
    renderer_.drawString(x_centro, y_ini++, "   | | |  _   / ___ \\  |\\  |.  \\___) |  |  _|| |_| |  _ < ", COLOR_VERDE);
    renderer_.drawString(x_centro, y_ini++, "   |_| |_| |_/_/   \\_\\_| \\_|_|\\_\\____/  |_|   \\___/|_| \\_\\", COLOR_VERDE);
    
    renderer_.drawString(x_centro+8, y_ini++, "  ____  _        _ __   _____ _   _  ____ ", COLOR_AMARILLO);
    renderer_.drawString(x_centro+8, y_ini++, " |  _ \\| |      / \\\\ \\ / /_ _| \\ | |/ ___|", COLOR_AMARILLO);
    renderer_.drawString(x_centro+8, y_ini++, " | |_) | |     / _ \\\\ V / | ||  \\| | |  _ ", COLOR_AMARILLO);
    renderer_.drawString(x_centro+8, y_ini++, " |  __/| |___ / ___ \\| |  | || |\\  | |_| |", COLOR_AMARILLO);
    renderer_.drawString(x_centro+8, y_ini++, " |_|   |_____/_/   \\_\\_| |___|_| \\_|\\____|", COLOR_AMARILLO);

    renderer_.drawString(50, 19, "GRACIAS POR JUGAR!");
    renderer_.drawString(39, 20, "PRESIONE ENTER O ESC PARA FINALIZAR.");
}
