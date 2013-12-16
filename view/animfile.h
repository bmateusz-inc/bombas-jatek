#ifndef ANIMFILE_H
#define ANIMFILE_H

#include <string>
#include <fstream>
#include <vector>

#include "../utils.h"

/** Animation frame */
struct Frame
{
    /** Delay after previous frame */
    TIME time;
    
    /** Graphic to show */
    std::string obj;
    
    /**
     * Constructor
     * @param iobj graphic to show
     * @param itime delay after previous
     */
    Frame(std::string iobj, TIME itime) : time(itime), obj(iobj) {}
};

/** Descriptor of a static animation */
class AnimFile
{
    private:
        std::vector<Frame> frames;
    public:
        /** Default constructor */
        AnimFile() {};
        
        /**
         * Constructor, loads animation phases
         * @param filename name of animation file (.anm)
         */
        AnimFile(std::string filename);
        
        /**
         * Calculate the current animation phase to show depending on
         * current frame time
         * @param time frame time
         * @return graphic name (full draw path)
         */
        std::string frame(TIME time);
};

#endif
