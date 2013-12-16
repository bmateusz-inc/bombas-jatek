#include "monster.h"

Monster::Monster(int itype, int isb, Point<float> ipoint, int iid, TIME inow)
        : type(itype), sb(isb), point(ipoint), prev(inow), id(iid)
{
    printf("Monster(%d)\n",id);
    if(type==0)
    {
        goesThroughWall=true;
        speed=Config::getInstance().CFG_SLOW_SPEED;
    }
    if(type==1)
    {
        goesThroughWall=false;
        speed=Config::getInstance().CFG_SLOW_SPEED;
    }
    now = 0;
    changeAnim = prev + Config::getInstance().CFG_RESPAWN_MONSTER;
    changeDirection = prev;
    stateAnim = 0;
    switch(rand()%4)
    {
        case 0: direction.setX(1); break;
        case 1: direction.setX(-1); break;
        case 2: direction.setY(1); break;
        default: direction.setY(-1); break;
    }
}

bool Monster::isActive()
{
    return now!=0;
}

bool Monster::collide()
{
    return false;
}

std::string Monster::die()
{
    speed=0;
    if(type==0) return "monster.anm#monster0";
    if(type==1) return "monster.anm#monster1";
    return "";
}

bool Monster::alive()
{
    return speed != 0 && now != 0;
}

void Monster::move(TIME inow)
{
    TIME delay = inow - now;
    if(now==0)
    {
        if(changeAnim < inow) now = inow;
        return;
    }
    prevPoint.setX(point.getX());
    prevPoint.setY(point.getY());
    setX(point.getX() + delay * direction.getX() * speed);
    setY(point.getY() + delay * direction.getY() * speed);
    prev = now;
    now = inow;
}

bool Monster::change()
{
    int ax = prevPoint.getX()/sb, bx = point.getX()/sb;
    int ay = prevPoint.getY()/sb, by = point.getY()/sb;
    if(ax != bx) return true;
    if(ay != by) return true;
    return false;
}

void Monster::setX(float x)
{
    point.setX(x);
}

void Monster::setY(float y)
{
    point.setY(y);
}

void Monster::setDx(float dx)
{
    direction.setX(dx);
    direction.setY(0);
    changeDirection = now;
}

void Monster::setDy(float dy)
{
    direction.setY(dy);
    direction.setX(0);
    changeDirection = now;
}

Point<float> Monster::getPoint() { return point; }
Point<int> Monster::getPointInt() { return Point<int>((0.5+point.getX())/sb, (0.5+point.getY())/sb); }

float Monster::getX() const { return point.getX(); }
float Monster::getY() const { return point.getY(); }

std::string Monster::state()
{
    if(now==0) return "";
    std::string ret = "monster";
    if(type==0) ret.append("0");
    if(type==1) ret.append("1");
    if(now - changeAnim > 500)
    {
        stateAnim++;
        changeAnim = now;
        if(stateAnim>1) stateAnim = 0;
    }
    if(stateAnim == 1) ret.append("_2");
    ret.append(".png");
    return ret;
}

bool Monster::getGoesThroughWall()
{
    return goesThroughWall;
}

bool Monster::getMove(Point<float>& p)
{
    if (changeDirection > prev)
    {
        p = point;
        return true;
    }
    return false;
}

int Monster::getId()
{
    return id;
}

float Monster::getNetworkX() const
{
    float pos = roundf(point.getX() / sb) * sb;
    pos += direction.getX()*5;
    return pos;
}

float Monster::getNetworkY() const
{
    float pos = roundf(point.getY() / sb) * sb;
    pos += direction.getY()*5;
    return pos;
}

void Monster::setNetworkX(float ix)
{
    float pos = roundf(ix / sb) * sb;
    direction.setX(0);
    if (ix > pos + epsilon) { direction.setX(1); std::cout<< id << " x " << 1 << "\n"; }
    if (ix < pos - epsilon) { direction.setX(-1); std::cout<< id << " x " << -1 << "\n";}
    point.setX(pos);
    prevPoint.setX(pos);
}

void Monster::setNetworkY(float iy)
{
    float pos = roundf(iy / sb) * sb;
    direction.setY(0);
    if (iy > pos + epsilon) { direction.setY(1); std::cout<< id << " y " << 1 << "\n"; }
    if (iy < pos - epsilon) { direction.setY(-1); std::cout<< id << " y " << -1 << "\n"; }
    point.setY(pos);
    prevPoint.setY(pos);
}
