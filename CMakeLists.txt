cmake_minimum_required(VERSION 3.10)
project(NaveEspacial)

set(CMAKE_CXX_STANDARD 17)

add_executable(NaveEspacial_Refactored
    src/main.cpp
    src/Game.cpp
    src/InputManager.cpp
    src/Renderer.cpp
    src/SpaceShipLogic.cpp
    src/MeteorManager.cpp
    src/CollisionEngine.cpp
)

target_include_directories(NaveEspacial_Refactored PRIVATE include)
