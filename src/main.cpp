//g++ -std=c++17 main.cpp Game.cpp Button.cpp HomeScreen.cpp SelectMapScreen.cpp SelectModeScreen.cpp GameScreen.cpp ResourceManager.cpp Screen.cpp OverlayScreen.cpp -o main -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
#include "../headers/Game.hpp"

int main() {
    Game Minesweeper;
    Minesweeper.Run();
}