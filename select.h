#ifndef SELECT_H
#define SELECT_H

#include "view/view.h"
#include "input.h"
#include "gametable.h"
#include "network.h"
#include "textinput.h"

/**
 * This class allows the users to select controllers before starting the game
 */
class Select
{
    private:
        View &view;
        Input &input;
        GameTable &table;
        Network &network;
        GAME_MODE gameMode;
        GAME_PLACE gamePlace;
        
        std::vector<int> team;
        std::vector<int> place;
        bool run;
        int n;
        int minimalPlayers;
        
        int clientId;
        bool searchServer;
        bool hasError;
        
        /** Bind players to table and control */
        void bindPlayers();
        
        void loopLocal();
        void loopServer();
        void loopClientSearch();
        void loopClient();
        void drawNetwork();
        
    public:
        /**
         * Constructor
         * @param iview View class
         * @param iinput Input class
         * @param itable Table class
         * @param iNetwork Network class
         * @param igameMode game mode selected
         * @param igamePlace game place
         */
        Select(View &iview, Input &iinput, GameTable &itable, Network &iNetwork,
               GAME_MODE igameMode, GAME_PLACE igamePlace);
        
        /**
         * The main loop, shows the menu for selecting input
         * @return selecting is not finished
         */
        bool mainLoop();
        
        /** Getter for run */
        bool getRun() { return run; }
        
        /** Getter for number of players */
        int getN() { return input.getNControllers(); }
        
        /** Getter of game mode */
        GAME_MODE getGameMode() const { return gameMode; }
};

#endif
