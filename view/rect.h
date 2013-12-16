#ifndef RECT_H
#define RECT_H

#include <string>

/** This class stores a rectangle with float coordinates and dimensions */
struct Rect
{
    public:
        /** Coordinate X */
        float x;
        
        /** Coordinate Y */
        float y;
        
        /** Width */
        float w;
        
        /** Height */
        float h;

        /** Default constructor */
        Rect() {}
        
        /**
         * Constructor
         * @param ix coordinate
         * @param iy coordinate
         * @param iw width
         * @param ih height
         */
        Rect(float ix, float iy, float iw, float ih) :
            x(ix), y(iy), w(iw), h(ih) {}
};

#endif
