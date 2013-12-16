#include "flame.h"

Flame::Flame(int x, int y, TIME itime, int idirection, int iOwner)
    : place(Point<int>(x,y)), time(itime), direction(idirection), owner(iOwner) {}

int Flame::getX() { return place.getX(); }

int Flame::getY() { return place.getY(); }

Point<int> Flame::getPoint() { return place; }

int Flame::getDirection() { return direction; }

int Flame::getOwner() { return owner; }

std::string Flame::state()
{
    if(direction == -1) return "";
    if(direction == 7)
    {
        direction = -1;
        return "brick_flame.anm";
    }
    
    std::string ret = "flame_3.png";

    if(diff > 100) ret = "flame_2.png";
    if(diff > 200) ret = "flame.png";
    if(diff > 300) ret = "flame_2.png";
    if(direction>7) ret.insert(5,"_end");
    if(direction%2) ret.append("#r");
    if(direction==9) ret.append("v");
    if(direction==12) ret.append("#v");
    return ret;
}

bool Flame::expire(TIME now)
{
    diff = time - now;
    return time <= now;
}
