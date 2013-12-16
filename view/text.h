#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>

#include "object.h"
#include "matrix.h"
#include "utf8.h"

/**
 * Class for text handling using Multi class
 */
class Text : public Object
{
    private:
        /** Matrix of characters (in order of the picture) */
        Matrix<Utf8> m;
        
        /** Link on graphics map from View */
        std::map<std::string, Object*> *graphs;
        
        /** Name of file */
        std::string name;
        
        /** Load resources */
        bool load(std::string fileName);

    public:
        /** Default constructor */
        Text() { graphs = 0; }
        
        /**
         * Constructor, load resources
         * @param fileName name of txh file
         * @param igraphs graphics from view class
         */
        Text(std::string fileName, std::map<std::string, Object*> *igraphs) : graphs(igraphs) { load(fileName); }

        /** Draw text (font.txh#my_text) */
        void draw(float x, float y, float w, float h, std::string param);
};

#endif
