#ifndef SCOREBOARD_H
#define SCOREBOARD_H

#include "view/view.h"
#include "input.h"
#include "gametable.h"
#include "utils.h"

/** Show results of a round */
class ScoreBoard
{
    private:
        View &view;
        Input &input;
        GameTable &table;
        TIME stop;
        bool skip;
        bool endOfGame;

    public:
        /**
         * Constructor
         * @param iView View class
         * @param iInput Input class
         * @param iTable Table class
         */
        ScoreBoard(View &iView, Input &iInput, GameTable &iTable);
        
        /**
         * Draws the scoreboard on screen after a round
         * @return is not finished
         */
        bool mainLoop();
        
        /** True if one of the players reached the score limit */
        bool getEndOfGame() const { return endOfGame; }
};

#endif
