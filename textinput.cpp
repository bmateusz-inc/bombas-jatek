#include "textinput.h"

bool TextInput::mainLoop()
{
    view.draw(0,0,640,480,"menu.png");
    
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            case SDL_QUIT:
                input.erase();
                return false;
                break;

            case SDL_KEYDOWN:
            {
                Uint16 key = e.key.keysym.unicode;
                if (key == SDLK_BACKSPACE) { if (!input.empty()) input.erase(input.size()-1); }
                else if (key == SDLK_RETURN) return false;
                else if (key == SDLK_ESCAPE) { input.erase(); return false; }
                else 
                {
                    if( e.key.keysym.unicode < 0x80 && e.key.keysym.unicode > 0 ){
                        input.append((char*)&(e.key.keysym.unicode));
                    }
                }
                break;
            }
        }
    }
    
    const int margin = 5;
    
    view.draw(60-margin, 120-margin, (input.size()+1)*20+margin*2, 40+margin*2, "tween.mlt#16 0");
    view.draw(60, 120, 20, 40, "font_white.txh#" + input);
    
    view.draw(0, 60, 640, 40, "tween.mlt#16 0");
    view.draw(50, 60, 20, 40, "font_white.txh#Csatlakozás:");
    
    if (GetTicks() % 1000 < 500) view.draw(60+input.size()*20, 150, 20, 10, "tween.mlt#8 0");
    
    view.swap();
    return true;
}
