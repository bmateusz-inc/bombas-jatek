#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>
#include <vector>

#include "control.h"
#include "view/view.h"
#include "utils.h"

/** Input handling, with SDL. Do not forget to call poll() */
class Input
{
    private:
        std::vector<Control> controls;
        Control universalControl;
        std::vector<SDL_Joystick*> joystick;
        int nJoy;
        View* view;
        bool run;
        bool round;
        bool mode;
        
        void updateUniversalControl();

    public:
        /** Constructor, initialize controls */
        Input();
        
        /** Destructor, release joysticks */
        ~Input();
        
        /** Update joysticks, search for newly connected */
        void updateJoySticks();

        /** Program should run */
        bool getRun() { return run; }
        
        /** Program should step back one level */
        bool getRunSoft() { if (run == false) { run = true; return false; } return run; }
        
        /** Get number of joysticks */
        int getNJoy() { return nJoy; }
        
        /** Get number of players*/
        int getNControllers() { return nJoy+2; }

        /** Set run */
        void setRun(bool irun) { round=run=irun; }
        
        /** Set View class */
        void setView(View* iView) { view=iView; }
        
        /**
         * Set mode:
         * 0: menu,
         * 1: in-game
         */
        void setMode(bool m) { mode = m; }

        /** Get Control class of selected ID */
        Control* operator()(int i) { return &(controls[i]); }
        
        /** Get universal control class */
        Control* operator()() { return &universalControl; }

        /** Poll events with SDL */
        void poll(TIME delay);

        /** Set delay between frames */
        void setDelay(TIME delay);
        
        /** Set all fire buttons to false */
        void noPut();
};

#endif
