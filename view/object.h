#ifndef OBJECT_H
#define OBJECT_H

#include <string>

/**
 * Base class of drawable objects
 */
class Object
{
    public:
        /**
         * Drawing object on screen
         * @param x coordinate
         * @param y coordinate
         * @param w width
         * @param h height
         * @param param full drawing string
         */
        virtual void draw(float x, float y, float w, float h, std::string param = "") {}
        
        /** Bind graphic to draw texture */
        virtual void bind() {}
        
        /** Destructor */
        virtual ~Object() {}
};

#endif
