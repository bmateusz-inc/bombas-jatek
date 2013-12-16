#ifndef GAMECONFIG_H
#define GAMECONFIG_H

#include <vector>
#include <string>

#include "gamemode.h"

/** GameTable configuration */
class GameConfig
{
    public:
        /** Number of columns on map */
        int sx;
        
        /** Number of rows on map */
        int sy;
        
        /** A squares size on map */
        int sb;
        
        /** Generate map */
        bool gen;
        
        /** Initialize map */
        bool init;
        
        /** Number of viruses on map */
        int nVirus;
        
        /** Number of bomb size boosters on map */
        int nBomb;
        
        /** Number of flame size boosters on map*/
        int nFlame;
        
        /** Goal of game (kill or monster) */
        GAME_GOAL goal;
        
        /** Place of game */
        GAME_PLACE place;
        
        /** Reset died, kill, killed and monsterKill variables at start */
        bool resetPlayer;

        /** Default constructor */
        GameConfig() : gen(true), init(true), nVirus(8), nBomb(8), nFlame(8), goal(GOAL_KILL), resetPlayer(true) {}
};

#endif
