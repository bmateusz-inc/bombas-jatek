#ifndef GAME_H
#define GAME_H

#include "view/view.h"
#include "input.h"
#include "network.h"
#include "gametable.h"
#include "menu.h"
#include "gamemanager.h"
#include "scoreboard.h"
#include <ctime>

/** The main loop of the game */
class Game
{
    private:
        View view;
        Input input;
        Network network;
        void initGame();

    public:
        Game();
        ~Game();
};

#endif
