#ifndef FLAME_H
#define FLAME_H

#include <string>

#include "point.h"
#include "utils.h"

/** Flame ignited by bombs, lasts for a few milliseconds */
class Flame
{
    private:
        Point<int> place;
        TIME time;
        TIME diff;
        int direction;
        int owner;

    public:
        /** Default constructor */
        Flame() {}
        
        /**
         * Constructor, initialize values
         * @param x coordinate
         * @param y coordinate
         * @param itime flame expire time
         * @param idirection direction, see getDirection()
         * @param iOwner player ID
         */
        Flame(int x, int y, TIME itime, int idirection, int iOwner);

        /** Get X coordinate */
        int getX();
        
        /** Get Y coordinate*/
        int getY();
        
        /** Get point*/
        Point<int> getPoint();
        
        /**
         * Get direction defined.
         * @return direction
         */
        int getDirection();
        
        /** Return owner of flame */
        int getOwner();
        
        /** Get animation state, returns draw string */
        std::string state();
        
        /**
         * Whether flame is expired or not
         * @param now current frame time
         */
        bool expire(TIME now);

        /** Equality operator */
        friend bool operator==(Flame& a, Point<int>& b)
        {
            return a.place == b;
        }
};

#endif
