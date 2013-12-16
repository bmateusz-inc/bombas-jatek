#include "input.h"

Input::Input()
{
    view=0;
    nJoy=0;
    controls.resize(6);
    joystick.resize(6);
    run=round=true;
    //updateJoySticks();
}

Input::~Input()
{
    for(int i=0; i<nJoy; ++i) SDL_JoystickClose(joystick[i]);
}

void Input::updateJoySticks()
{
    for(int i=0; i<nJoy; ++i) SDL_JoystickClose(joystick[i]);
    SDL_QuitSubSystem(SDL_INIT_JOYSTICK);
    SDL_InitSubSystem(SDL_INIT_JOYSTICK);
    SDL_JoystickEventState(SDL_ENABLE);
    SDL_JoystickUpdate();
    nJoy = SDL_NumJoysticks();
    printf("numOfJoys: %d\n",nJoy);
    for(int i=0; i<nJoy; ++i) joystick[i] = SDL_JoystickOpen(i);
}

void Input::poll(TIME delay)
{
    SDL_Event e;

    while (SDL_PollEvent(&e))
    {
        switch(e.type)
        {
            //case SDL_MOUSEBUTTONUP:
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym)
                {
                    case SDLK_p:
                        {
                            view->Screenshot();
                        }
                        break;
                    case SDLK_r:
                        {
                            view->loadPackage("img.ls");
                        }
                        break;
                    case SDLK_o:
                        {
                            view->Toggle();
                        }
                        break;

                    case SDLK_ESCAPE:
                        setRun(false);
                        break;

                    case SDLK_LEFT:
                        controls[0].setX(-1);
                        break;
                    case SDLK_RIGHT:
                        controls[0].setX(1);
                        break;
                    case SDLK_UP:
                        controls[0].setY(-1);
                        break;
                    case SDLK_DOWN:
                        controls[0].setY(1);
                        break;
                    case SDLK_RCTRL:
                    case SDLK_RETURN:
                        controls[0].setPut(true);
                        break;

                    case SDLK_a:
                        controls[1].setX(-1);
                        break;
                    case SDLK_d:
                        controls[1].setX(1);
                        break;
                    case SDLK_w:
                        controls[1].setY(-1);
                        break;
                    case SDLK_s:
                        controls[1].setY(1);
                        break;
                    case SDLK_TAB:
                        controls[1].setPut(true);
                        break;

                    default:
                        break;
                }
                break;

            case SDL_KEYUP:
                switch(e.key.keysym.sym)
                {
                    case SDLK_LEFT:
                        if(controls[0].getX()==-1) controls[0].setX(0);
                        break;
                    case SDLK_RIGHT:
                        if(controls[0].getX()==1) controls[0].setX(0);
                        break;
                    case SDLK_UP:
                        if(controls[0].getY()==-1) controls[0].setY(0);
                        break;
                    case SDLK_DOWN:
                        if(controls[0].getY()==1) controls[0].setY(0);
                        break;
                    case SDLK_RCTRL:
                    case SDLK_RETURN:
                        controls[0].setPut(false);
                        break;

                    case SDLK_a:
                        if(controls[1].getX()==-1) controls[1].setX(0);
                        break;
                    case SDLK_d:
                        if(controls[1].getX()==1) controls[1].setX(0);
                        break;
                    case SDLK_w:
                        if(controls[1].getY()==-1) controls[1].setY(0);
                        break;
                    case SDLK_s:
                        if(controls[1].getY()==1) controls[1].setY(0);
                        break;
                    case SDLK_TAB:
                        controls[1].setPut(false);
                        break;
                    default:
                        break;
                }
                break;

            case SDL_JOYBUTTONDOWN:
                {
                    int idx = e.button.which + 2;
                    printf("joydown %d\n",idx);
                    controls[idx].setPut(true);
                }
                break;

            case SDL_JOYBUTTONUP:
                {
                    int idx = e.button.which + 2;
                    controls[idx].setPut(false);
                }
                break;

            case SDL_JOYAXISMOTION:
                {
                    int idx = e.jaxis.which + 2;
                    if ( e.jaxis.axis == 0)
                    {
                        if( e.jaxis.value > 3200 ) controls[idx].setX( (float)e.jaxis.value / 32767 );
                        else if( e.jaxis.value < -3200 ) controls[idx].setX( (float)e.jaxis.value / 32768 );
                        else controls[idx].setX(0);
                    }
                    if ( e.jaxis.axis == 1)
                    {
                        if( e.jaxis.value > 3200 ) controls[idx].setY( (float)e.jaxis.value / 32767 );
                        else if( e.jaxis.value < -3200 ) controls[idx].setY( (float)e.jaxis.value / 32768 );
                        else controls[idx].setY(0);
                    }
                    printf("%d - %d - %d\n",idx,e.jaxis.axis,e.jaxis.value);
                }
                break;
                
            case SDL_QUIT:
                setRun(false);
                break;

            case SDL_VIDEORESIZE:
                if(view != 0) view->resize_event(e.resize.w, e.resize.h);
                break;
        }
    }
    setDelay(delay);
    updateUniversalControl();
}

void Input::setDelay(TIME delay)
{
    for(int i=0; i<nJoy+2; ++i)
    controls[i].setDelay(delay);
    universalControl.setDelay(delay);
}

void Input::updateUniversalControl()
{
    universalControl.reset();
    
    for(int i=0; i<nJoy+2; ++i) {
        universalControl += controls[i];
    }
}

void Input::noPut()
{
    for(int i=0; i<nJoy+2; ++i) {
        controls[i].setPut(false);
    }
    universalControl.setPut(false);
}