#ifndef VIEW_H
#define VIEW_H

#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
#include <map>
#include <string>

#include "sprite.h"
#include "multi.h"
#include "text.h"
#include "animobject.h"
#include "animation.h"
#include "animfile.h"
#include "../point.h"
#include "../utils.h"

/**
 Main class for handling graphics
 */
class View
{
    private:
        /** Map containing all the drawable objects */
        std::map<std::string, Object*> graphs;
        
        /** Map containing animation files */
        std::map<std::string, AnimFile> anims;
        
        /** Deque containing running animations */
        std::deque<Animation> anim;
        
        /** SDL surface for screen */
        SDL_Surface* screen;
        
        /** Desktop dimension */
        Point<int> desktop;
        
        /** Boolean weather keep ratio or not when resizing */
        bool keep_ratio;
        
        /** Take screenshot of current frame */
        bool screenshot;
        
        /** Toggle fullscreen after frame */
        bool toggle;
        
        /** Function to perform screenshot routine */
        void Screenshot_do();


        /** Time of previous frame */
        TIME prev;
        
        /** Time of current frame */
        TIME now;

        /** Screen refresh interval */
        int TICK_INTERVAL;

        /** Load .mlt file */
        bool loadMulti(std::string fileName);
        
        /** Load .txh file */
        bool loadText(std::string fileName);
        
        /** Load .anm file */
        void loadAnim(std::string anim);
        
        /**
         * Get resource file name from draw string
         * @param str full draw string (filename#params)
         * @return file name
         */
        std::string getRes(std::string str);
        
        /**
         * Get parameters from draw string
         * @param str full draw string (filename#params)
         * @return parameters
         */
        std::string getParam(std::string str);
        
        /** Get screen width */
        int getW() { return screen->w; }
        
        /** Get screen height */
        int getH() { return screen->h; }
        
        /** Toggle fullscreen */
        void toggleFullScreen();
        
    public:
        /** Constructor, initialize screen */
        View();
        
        /** Destructor, unload objects */
        ~View();
        
        /** Swap buffers, draw on screen */
        void swap();
        
        /**
         * Load image, animation or text file
         * @param fileName name of file, extension determines the type
         * @return success
         */
        bool load(std::string fileName);
        
        /**
         * Load resources from list
         * @param fileName name of package
         * @return success
         */
        bool loadPackage(std::string fileName);
        
        /**
         * Draw on frame
         * @param r rectangle of drawing position
         * @param res full drawing string
         */
        void draw(Rect r, std::string res);
        
        /**
         * Draw on frame
         * @param x coordinate
         * @param y coordinate
         * @param w width
         * @param h height
         * @param res full drawing string
         */
        void draw(float x, float y, float w, float h, std::string res);

        /**
         * Resize callback function
         * @param w width
         * @param h height
         */
        void resize_event(int &w, int &h);

        /** Draw animations on frame, delete ended animations */
        void drawAnim();

        /** Stop all animations */
        void stopAnim();
        
        /** Get current frame time */
        inline TIME getNow() { return now; }
        
        /** Set current frame time to actual time */
        void setNow() { now = GetTicks(); }
        
        /** Set refresh rate */
        void setTickInterval(int i) { TICK_INTERVAL = i; }
        
        /** Request to take screenshot at the end of frame */
        void Screenshot();
        
        /** Request to toggle fullscreen */
        void Toggle() { toggle = true; }

        /** Begin of frame, actualilze time variables for current and previous frame */
        void frameBegin();
        
        /** End of frame, sleep for a while to save CPU time */
        void frameEnd();
        
        /** Amount of sleep needed for enough frames per second */
        TIME frameDelay();
};

void SwapBuffers();
void TerminateOpenGL();
int InitOpenGL();


#endif
