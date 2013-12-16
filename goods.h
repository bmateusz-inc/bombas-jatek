#ifndef GOODS_H
#define GOODS_H

#include <string>
#include "player.h"
#include "point.h"

enum GOODS { FLAME = 12, BOMB, VIRUS};

/** Bonuses (bomb and flame booster) and viruses */
class Goods
{
    private:
        Point<int> point;
        GOODS type;

    public:
        /** Default constructor */
        Goods() {};
        
        /**
         * Constructor
         * @param ipoint place of good
         * @param itype type of good (virus, bomb+, flame+)
         */
        Goods(Point<int> ipoint, GOODS itype) : point(ipoint), type(itype) {};

        /** Get X coordinate */
        int getX() { return point.getX(); }
        
        /** Get Y coordinate */
        int getY() { return point.getY(); }
        
        /** Get point */
        Point<int> getPoint() { return point; }
        
        /** Get type of good */
        GOODS getType() { return type; }
        
        /** Get graphic file name of good */
        std::string state();
        
        /**
         * Apply effect on player
         * @param p pointer to player
         * @param text to show
         * @return code of infection
         */
        int effect(Player *p, std::string &text);
        
        /**
         * Effect player with good
         * @param p player
         * @param infection infection
         * @return text to show on table
         */
        static std::string effect(Player *p, int infection);
        
        /** Random virus */
        PLAYER_VIRUS getRandVirus();
        
        /** Get a random Goods element (just type) */
        static GOODS getRandGood()
        {
            return GOODS(FLAME + rand() % 3);
        }
};

#endif
