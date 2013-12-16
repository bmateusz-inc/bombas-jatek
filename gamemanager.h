#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <vector>

#include "gameconfig.h"
#include "gametable.h"
#include "view/view.h"
#include "input.h"
#include "gamemode.h"
#include "select.h"
#include "network.h"
#include "msgbox.h"

/** Handle events on the GameTable, regarding the game mode */
class GameManager
{
    private:
        GAME_MODE gameMode;
        GAME_PLACE gamePlace;
        View &view;
        Input &input;
        GameTable &table;
        Network &network;
        TIME stop;
        TIME spawn_monster;
        int alivePlayers;
        int maxPoint;
        int maxKills;
        int maxMonsterKills;
        std::string currMap;
        std::string nextMap;
        bool round;
        bool showScoreBoard;
        bool loadNextMap;
        
        void doNetwork();

    public:
        /**
         * Constructor
         * @param iGameMode game mode
         * @param iGamePlace place of game (local, server, client)
         * @param iview View class
         * @param iInput Input class
         * @param iTable Table class
         * @param iNetwork Network class
         */
        GameManager(GAME_MODE iGameMode,
                    GAME_PLACE iGamePlace,
                    View &iview,
                    Input &iInput,
                    GameTable &iTable,
                    Network &iNetwork);
        
        ~GameManager();
        
        /** Prepare game table */
        void prepareTable();
        
        /** Start new round, reset table */
        void newRound();
        
        /** Check state of game (score of players) and stop if necessary */
        void checkState();
        
        /** Check after frames */
        void checkBefore();
        
        /** Check after rounds */
        void checkAfter();
        
        /** Wait for round start */
        void waitRound();
        
        /** End of this game */
        bool getRun();
        
        /** End of round */
        bool getRound();
        
        /** Show scoreboard after round finished or not */
        bool getShowScoreBoard();
};

#endif
