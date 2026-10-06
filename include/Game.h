#pragma once
#include "InputManager.h"
#include "Renderer.h"
#include "SpaceShipLogic.h"
#include "MeteorManager.h"
#include "CollisionEngine.h"

enum class GameState {
    MAIN_MENU,
    CONTROLS_MENU,
    PLAYING,
    PAUSED,
    EXPLODING,
    GAME_OVER,
    EXIT
};

class Game {
public:
    Game();
    ~Game();

    void run();

private:
    void handleInput();
    void update();
    void render();

    void updateMainMenu();
    void updateControlsMenu();
    void updatePlaying();
    void updatePaused();
    void updateExploding();
    void updateGameOver();

    void drawBorder();
    void drawHUD();
    void drawMainMenu();
    void drawControlsMenu();
    void drawPauseMenu();
    void drawGameOverMenu();
    void drawFinalScreen();

    InputManager inputManager_;
    Renderer renderer_;
    SpaceShipLogic ship_;
    MeteorManager meteors_;
    CollisionEngine collisionEngine_;

    GameState currentState_;
    InputKey currentInput_;
    bool isRunning_;

    int explosionFrameTimer_;
};
