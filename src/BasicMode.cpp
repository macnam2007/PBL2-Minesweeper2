#include "../headers/BasicMode.hpp"
#include "../headers/SquareBoard.hpp"
#include "../headers/Player.hpp"
#include "../headers/EnumOpenResult.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

BasicMode::BasicMode(AbstractBoard* board, Player* player)
    : GameMode(board,player) {}