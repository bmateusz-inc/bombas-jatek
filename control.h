#ifndef CONTROL_H
#define CONTROL_H

#include "point.h"
#include "utils.h"
#include <cstdio>

const float cos45 = 0.707106781;

/** Interface between Input and Player */
class Control
{
    private:
        /** Local */
        Point<float> delta; //or previous if remote
        bool put;

        /** Time between current and previous frame */
        float delay;
        
    public:
        /** Constructor */
        Control()
        {
            delay=0;
            put=false;
        }
        
        /**
         * Get new X coordinate regarding current position, speed and frame time
         * @param ix current X position
         * @param speed speed factor
         * @return new X position
         */
        float px(float ix, float speed) const
        {
            return ix + (getY()==0 ? getX() : cos45*getX()) * speed * delay;
        }
        
        /**
         * Get new Y coordinate regarding current position, speed and frame time
         * @param iy current Y position
         * @param speed speed factor
         * @return new Y position
         */
        float py(float iy, float speed) const
        {
            return iy + (getX()==0 ? getY() : cos45*getY()) * speed * delay;
        }

        /** Set X vector for local player */
        void setX(float x) { delta.setX(x); }
        
        /** Set Y vector for local player */
        void setY(float y) { delta.setY(y); }
        
        /** Set time difference between frames */
        void setDelay(float d) { delay = d; }
        
        /** Set if the player wants to put bomb */
        void setPut(bool p) { put = p; }

        /** Get X vector */
        float getX() const { return delta.getX(); }
        
        /** Get Y vector */
        float getY() const { return delta.getY(); }
        
        /** Get time difference between frames */
        float getDelay() const { return delay; }
        
        /** Get if the player wants to put bomb */
        bool getPut() const { return put; }
        
        /** Set all values to default */
        void reset() { delta.setX(0); delta.setY(0); put = false; }
        
        /** Implicit add operator */
        void operator+= (const Control& control)
        {
            delta.setX(control.getX() + delta.getX());
            delta.setY(control.getY() + delta.getY());
            put |= control.getPut();
        }
};

#endif
