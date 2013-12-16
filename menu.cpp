#include "menu.h"

Menu::Menu(View &iView, Input &iInput) : view(iView), input(iInput)
{
    changed = 0;
    selected = 0;
    mode = GAME_MODE_NONE;
    place = LOCAL;
    view.setNow();
    started = view.getNow();
    
    input.noPut();
    items.push_back(MenuItem("Történet",0,SINGLE));
    items.push_back(MenuItem("Klasszikus",0,CLASSIC));
    items.push_back(MenuItem("Csapat",0,TEAM));
    items.push_back(MenuItem("Vadászat",0,SURVIVE));
    items.push_back(MenuItem("Helyi játék / Szerver",0,GAME_MODE_LOCAL));
    //items.push_back(MenuItem("Szerver létrehozása",0,GAME_MODE_SERVER));
    items.push_back(MenuItem("Csatlakozás",0,GAME_MODE_CLIENT));
    items.push_back(MenuItem("Kilépés",0,EXIT));
}

void Menu::setMode(GAME_MODE t)
{
    mode = t;
}

GAME_MODE Menu::getMode()
{
    return mode;
}

GAME_PLACE Menu::getPlace()
{
    return place;
}

int MenuItem::length() const
{
    Utf8 utf8;
    std::stringstream ss(txt);
    int c = 0;
    while(ss >> utf8) ++c;
    return c;
}

bool Menu::mainLoop()
{
    view.setNow();
    view.draw(0,0,640,480,"menu.png");
    input.poll(0);
    if(input()->getY() == 0) changed = 0;
    if(changed < view.getNow())
    {
        if(input()->getY() < 0) up();
        else if(input()->getY() > 0) down();
    }
    
    int offsetX = started + 1000 - view.getNow();
    int offsetY = 30;
    if (offsetX < 0) offsetX = 0;
    
    for(int i=0; i < static_cast<int>(items.size()); ++i)
    {
        if (i == 4) offsetY += 20;
        if (i == selected) view.draw(offsetX, 20+50*i+offsetY, 640, 40, "tween.mlt#8 0");
        else view.draw(offsetX, 20+50*i+offsetY, 640, 40, "tween.mlt#16 0");
        view.draw(offsetX+40+1, 20+50*i+1+offsetY, 20, 40, "font_white.txh#" + items[i].txt);
        if (i != selected) view.draw(offsetX+40, 20+50*i+offsetY, 20, 40, "font.txh#" + items[i].txt);
    }
    view.drawAnim();
    view.swap();
    if(input()->getPut())
    {
        GAME_MODE selectedMode = items[selected].mode;
        if (selectedMode == GAME_MODE_LOCAL) { place = SERVER; swapServerLocal(); }
        else if (selectedMode == GAME_MODE_SERVER) { place = LOCAL; swapServerLocal(); }
        else if (selectedMode == GAME_MODE_CLIENT) { place = CLIENT; setMode(GAME_MODE_NONE); return false; }
        else
        {
            if (place == CLIENT) place = LOCAL;
            if (!(place == SERVER && selectedMode == SINGLE))
            {
                setMode(selectedMode);
                return false;
            }
        }
        input.noPut();
    }
    if(input.getRun()==false) return false;
    return true;
}

void Menu::swapServerLocal()
{
    if (items[4].mode == GAME_MODE_LOCAL)
    {
        items[4].set("Szerver létrehozása / Helyi",GAME_MODE_SERVER);
    } else {
        items[4].set("Helyi játék / Szerver",GAME_MODE_LOCAL);
    }
}


void Menu::down()
{
    selected++;
    check();
}

void Menu::up()
{
    selected--;
    check();
}

void Menu::check()
{
    changed = view.getNow()+300;
    if(selected < 0) selected = items.size()-1;
    if(selected >= static_cast<int>(items.size())) selected = 0;
}

void Menu::tween()
{
    view.draw(0,0,640,480,"fade_in.anm");
}
