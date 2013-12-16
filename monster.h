#ifndef MONSTER_H
#define MONSTER_H

#include <string>
#include <cmath>

#include "point.h"
#include "utils.h"
#include "config.h"

const float epsilon = 0.2;

/**
 * This class contains a monster
 */
class Monster
{
    private:
        int type;
        int sb;
        Point<float> point;
        Point<float> prevPoint;
        Point<float> direction;
        TIME now;
        TIME prev;
        TIME changeDirection;
        TIME changeAnim;
        int stateAnim;
        float speed;
        bool goesThroughWall;
        int id;

    public:
        /**
         * Constructor
         * @param itype type of monster
         * @param isb size of a brick on map, necessary for random way changes
         * @param ipoint starting point
         * @param iid monster id
         * @param inow current frame time
         */
        Monster(int itype, int isb, Point<float> ipoint, int iid, TIME inow);
        
        /** Not yet active, false for a few seconds when the class is created */
        bool isActive();

        /** Is colliding with wall */
        bool collide();

        /**
         * Dies
         * @return animation of dying
         */
        std::string die();

        /** Is alive */
        bool alive();

        /**
         * Move the monster, with calculating the difference between
         * current and previous frame time
         * @param inow current frame time
         */
        void move(TIME inow);

        /** Change direction */
        bool change();

        /** Set X coordinate */
        void setX(float x);
        
        /** Set Y coordinate */
        void setY(float y);
        
        /** Set direction X */
        void setDx(float dx);
        
        /** Set direction Y */
        void setDy(float dy);
        
        /**
         * Get movement of monster.
         * @param p the new position +/- 0.5 where it wants to go
         * @return the player changes it's position or not
         */
        bool getMove(Point<float>& p);

        /** Get exact point on screen */
        Point<float> getPoint();
        
        /** Get rounded point, which rectangle is the nearest */
        Point<int> getPointInt();
        
        /** Get exact X coordinate */
        float getX() const;
        
        /** Get exact Y coordinate */
        float getY() const;
        
        /** Get animation state */
        std::string state();
        
        /** Return that the monster can walk through walls or not */
        bool getGoesThroughWall();
        
        /** Get monster id */
        int getId();
        
        /** Get network x, which contains the place and <b>direction</b> of monster */
        float getNetworkX() const;
        /** Get network y, which contains the place and <b>direction</b> of monster */
        float getNetworkY() const;
        
        /** Set network x, which contains the place and <b>direction</b> of monster */
        void setNetworkX(float ix);
        /** Set network y, which contains the place and <b>direction</b> of monster */
        void setNetworkY(float iy);
};

#endif
