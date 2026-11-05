#pragma once

#include "CellPoint.hpp"
#include "EnumOpenResult.hpp"
#include "GameMode.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

class BasicMode : public GameMode
{
    public:
        BasicMode(AbstractBoard* board, Player* player);
};