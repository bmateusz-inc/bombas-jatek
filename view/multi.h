#ifndef MULTI_H_INCLUDED
#define MULTI_H_INCLUDED

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <string>
#include <map>
#include <fstream>
#include <sstream>

#include "rect.h"
#include "object.h"

/** Can draw parts of a Sprite */
class Multi : public Object
{
    private:
        GLfloat sx;
        GLfloat sy;
        std::string name;
        std::map<std::string, Object*> *graphs;
        bool load(std::string fileName);

    public:
        /** Default constructor */
        Multi() { graphs=0; }
        
        /**
         * Constructor
         * @param fileName name of multi file (.mlt)
         * @param igraphs list of loaded graphics from View class
         */
        Multi(std::string fileName, std::map<std::string, Object*> *igraphs) : graphs(igraphs) { load(fileName); }

        /**
         * Draw a specific slice of a Sprite
         * @param x coordinate
         * @param y coordinate
         * @param w width
         * @param h height
         * @param param full string param, determines which slice need to be drawn
         */
        void draw(float x, float y, float w, float h, std::string param);
};
#endif
