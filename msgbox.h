#ifndef MSGBOX_H
#define	MSGBOX_H

#include "view/view.h"
#include "input.h"
#include <string>

/** Show (error) message */
class Msgbox {
    private:
        std::string txt;
        View &view;
        Input &input;

    public:
        /** Constructor */
        Msgbox(std::string itxt, View &iview, Input &iinput);
};

#endif
