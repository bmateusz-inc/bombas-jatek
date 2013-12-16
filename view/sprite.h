#ifndef SPRITE_H
#define SPRITE_H

#include "lodepng/lodepng.h"

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <vector>
#include <string>

#include "rect.h"
#include "object.h"

//#define GL_CLAMP_TO_EDGE 0x812F

/**
 * Class for handle single drawable objects.
 */
class Sprite : public Object
{
    private:
        GLuint texture;
        GLenum texture_format;
        GLint  nColors;
        std::string fn;
        unsigned w,h;
        const float getW() const { return w; }
        const float getH() const { return h; }
        bool load(std::string fileName);

    public:
        /** Default constructor */
        Sprite() {};
        
        /**
         * Constructor
         * @param fileName name of graphic file (.png)
         */
        Sprite(std::string fileName) { load(fileName); };
        
        /** Destructor, free memory */
        ~Sprite();

        /**
         * Draw sprite on screen
         * @param x coordinate
         * @param y coordinate
         * @param w width
         * @param h height
         * @param param full draw string
         */
        void draw(float x, float y, float w, float h, std::string param = "");
        
        /** Bind graphic for drawing */
        void bind() { glBindTexture( GL_TEXTURE_2D, texture ); }
};

#endif
