#ifndef BOMB_H
#define BOMB_H

#include <string>

#include "point.h"
#include "player.h"
#include "utils.h"

/** Bomb on the game table */
class Bomb
{
    private:
        TIME activated;
        TIME now;
        Point<int> point;
        int size;
        Player* player;

    public:
        /** Default constructor */
        Bomb() { player=0; };
        
        /**
         * Constructor
         * @param iactivated when should it become ignited
         * @param ipoint place of bomb
         * @param iplayer Player who put it
         */
        Bomb(TIME iactivated, Point<int> ipoint, Player* iplayer) :
            activated(iactivated), now(0), point(ipoint), player(iplayer)
        {
            if(player != 0)
            {
                size = player->getSize();
                player->decBombs();
            }
        }

        /** Bomb ignites by another flame, update player's number of bombs */
        void boom()
        {
            if(player != 0) player->incBombs();
        }

        /**
         * Bomb ignites by time
         * @param inow current frame time
         * @return ignites or not
         */
        bool boom(TIME inow)
        {
            now = inow;
            if(now >= activated)
            {
                return true;
            } else return false;
        }

        /**
         * Animation state
         * @return graphic string
         */
        std::string state()
        {
            if((activated - now) % 1000 > 500) return "bomb.png";
            else return "bomb_2.png";
        }
        
        /** Get X coordinate */
        int getX() { return point.getX(); }
        
        /** Get Y coordinate */
        int getY() { return point.getY(); }
        
        /** Get place of bomb */
        Point<int> getPoint() { return point; }
        
        /** Get flame size */
        int getSize() { return size; }
        
        /**
         * Get owner
         * @return player ID
         */
        int getOwner()
        {
            if(player != 0) return player->getId();
            else { printf("Error, no player for bomb"); return 0; }
        }
};

#endif
