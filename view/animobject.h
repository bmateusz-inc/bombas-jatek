#ifndef ANIMOBJECT_H
#define ANIMOBJECT_H

#include <string>
#include <queue>
#include <map>

#include "object.h"
#include "animation.h"
#include "animfile.h"
#include "../utils.h"

/** Animation to use in draw */
class AnimObject : public Object
{
    private:
        std::deque<Animation>* anim;
        std::map<std::string, AnimFile>* anims;
        std::string name;
        TIME* now;
    public:
        /** Default constructor */
        AnimObject() { anim = 0; anims=0; }
        
        /**
         * Constructor
         * @param ianim deque of animations from view
         * @param ianims animation files
         * @param iname name of animation
         * @param inow pointer to current frame time
         */
        AnimObject(std::deque<Animation>* ianim, std::map<std::string, AnimFile>* ianims, std::string iname, TIME* inow)
        : anim(ianim), anims(ianims), name(iname), now(inow) {}

        /**
         * Draw animation
         * @param x coordinate
         * @param y coordinate
         * @param w width
         * @param h height
         * @param param graphic parameter
         */
        void draw(float x, float y, float w, float h, std::string param = "")
        {
            if(anim)
            {
                anim->push_back(Animation(Rect(x, y, w, h), *now, &((*anims)[name]), param));
            }
        }
};

#endif
