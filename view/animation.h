#ifndef ANIMATION_H
#define ANIMATION_H

#include <deque>
#include <string>

#include "rect.h"
#include "animfile.h"
#include "../utils.h"

/** Static animation */
class Animation
{
    private:
        Rect rect;
        TIME start;
        AnimFile* anim;
        std::string param;

    public:
        /** Default constructor */
        Animation() {}
        
        /**
         * Constructor
         * @param irect rectangle where the animation will be shown
         * @param istart start time of animation
         * @param ianim previously loaded animation file
         * @param iparam drawing parameters
         */
        Animation(Rect irect, TIME istart, AnimFile* ianim, std::string iparam = "")
        : rect(irect), start(istart), anim(ianim), param(iparam) {}

        /**
         * Draw animation frame regarding time
         * @param time current frame time
         * @return graphic string
         */
        std::string draw(TIME time)
        {
            std::string f = anim->frame(time-start);
            if(f.empty()) return "";
            if(param.empty()) return f;
            if(param[0]=='#') return f + param;
            else return param + f;
        }

        /** Rectangle where animation is drawn */
        Rect getRect() { return rect; }
};

#endif
