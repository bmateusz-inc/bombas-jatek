#ifndef TEXTINPUT_H
#define	TEXTINPUT_H

#include <SDL/SDL.h>
#include <string>
#include "view/view.h"

/** Get text from keyboard (ASCII only) */
class TextInput {
    private:
        std::string input;
        View &view;

    public:
        /**
         * Constructor
         * @param iview View class
         */
        TextInput(View &iview) : view(iview) { SDL_EnableUNICODE(1); }
        
        /** Desctructor */
        ~TextInput() { SDL_EnableUNICODE(0); }
        
        /** The main loop */
        bool mainLoop();
        
        /** After main loop ended, get entered text */
        std::string getInput() const { return input; }
};

#endif
