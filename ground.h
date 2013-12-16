#ifndef GROUND_H
#define GROUND_H

#include <iostream>
#include <string>

enum GROUND { ROAD='.', WALL='f', BLOCK='x', WIN='W' };
typedef unsigned char GROUND_TYPE;

/** Simple class to store the table's base elements, the ground */
class Ground
{
    private:
        GROUND_TYPE g;

    public:
        /** Default constructor */
        Ground() { g = ROAD; }
        
        /**
         * Constructor
         * @param ig ground type
         */
        Ground(GROUND_TYPE ig) : g(ig) {}

        /** Graphic file name of current ground */
        std::string state()
        {
            if(g==ROAD) return "path.png";
            if(g==WALL) return "brick.png";
            if(g==BLOCK) return "stone.png";
            if(g==WIN) return "win.png";
            return "";
        }
        
        /** Equality operator */
        Ground& operator=(const GROUND_TYPE &i)
        {
            g = i;
            return *this;
        }
        
        /** Check equality */
        friend bool operator==(const Ground& a, const Ground& b)
        {
            return a.g == b.g;
        }
        
        /** Check not equals */
        friend bool operator!=(const Ground& a, const Ground& b)
        {
            return a.g != b.g;
        }
        
        /** Input stream operator */
        friend std::istream& operator>>(std::istream &input, Ground &ground)
        {
            GROUND_TYPE c;
            input >> c;
            if(c == ROAD) ground.g = ROAD;
            if(c == WALL) ground.g = WALL;
            if(c == BLOCK) ground.g = BLOCK;
            if(c == WIN) ground.g = WIN;
            return input;
        }
        
        /** Output stream operator */
        friend std::ostream& operator<<(std::ostream &output, const Ground &ground)
        {
            output << (int) ground.g;
            return output;
        }
};

#endif
