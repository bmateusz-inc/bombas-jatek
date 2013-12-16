#ifndef MENU_H
#define MENU_H

#include <string>

#include "view/utf8.h"
#include "view/view.h"
#include "input.h"
#include "gametable.h"
#include "utils.h"
#include "gamemode.h"

/** Simple class, used by Menu */
class MenuItem
{
    public:
        /** Text to show */
        std::string txt;
        
        /** Next menu item when selected */
        int next;
        
        /** Game mode */
        GAME_MODE mode;
        
        /** Constructor */
        MenuItem(std::string inp, int inext, GAME_MODE imode)
                : txt(inp), next(inext), mode(imode)
        {}
        
        /** Equality operator */
        MenuItem& operator=(const MenuItem &copy)
        {
            txt = copy.txt;
            next = copy.next;
            mode = copy.mode;
            return *this;
        }
        
        /** Unicode length of string */
        int length() const;
        
        /** Setter */
        void set(std::string itxt, GAME_MODE imode)
        {
            txt = itxt;
            mode = imode;
        }
};

/** Menu, you can navigate on it and select from the menu items */
class Menu
{
    private:
        View &view;
        Input &input;
        std::vector<MenuItem> items;
        TIME changed;
        int selected;
        GAME_MODE mode;
        GAME_PLACE place;
        TIME started;
        
        /** Set game mode */
        void setMode(GAME_MODE t);
        
        void swapServerLocal();

    public:
        /**
         * Constructor
         * @param iView View class
         * @param iInput Input class
         */
        Menu(View &iView, Input &iInput);
        
        /** Get selected game mode */
        GAME_MODE getMode();
        
        /** Get selected game mode */
        GAME_PLACE getPlace();
        
        /** Main loop, draws on screen and checks input */
        bool mainLoop();
        
        /** Go down one item */
        void down();
        
        /** Go up one item */
        void up();
        
        /** Check time between the last change of menu items and now, step
         to next if necessary */
        void check();
        
        /** Fade the screen from black */
        void tween();
};

#endif
