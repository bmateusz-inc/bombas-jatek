#include "msgbox.h"

Msgbox::Msgbox(std::string itxt, View &iview, Input &iinput)
             : txt(itxt), view(iview), input(iinput)
{
    bool run = true;
    input.noPut();
    while (run)
    {
        view.draw(0,0, 640, 480, "menu.png");
        view.draw(0,0, 640, 480, "tween.mlt#16 0");

        view.draw(50, 70, 20, 40, "font_white.txh#" + txt);
        
        input.poll(view.getNow());
        run = !input()->getPut();
        
        view.swap();
    }
    input.noPut();
}