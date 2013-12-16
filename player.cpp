#include "player.h"

Player::Player() {
    control = 0;
    score = kill = killed = monsterKill = 0;
    updated = 0;
    showKills = 0;
    sendShowKills = false;
    skin = "test";
    team = 1;
    remote = false;
    init();
}

Player::~Player() {}

void Player::init()
{
    put = false;
    life = 100;
    prevWay = STILL;
    change = now = step = 0;
    bombs = 1;
    size = 2;
    speed = Config::getInstance().CFG_DEFAULT_SPEED;
    died = 0;
    novirus();
}

void Player::newRound()
{
    kill = killed = monsterKill = 0;
}

void Player::novirus()
{
    speed = Config::getInstance().CFG_DEFAULT_SPEED;
    virus = 0;
    virusMode = NOVIRUS;
}

void Player::move(TIME inow)
{
    if(life <= 0) return;
    now = inow;
    
    if(virus != 0)
    {
        if(virus <= now) { novirus(); }
    }
    
    if (control != 0)
    {
        if(control->getDelay() > 0) prev = point;

        point.setX( control->px(point.getX(), speed) );
        point.setY( control->py(point.getY(), speed) );
        put = control->getPut();
    } // else printf("control = 0\n");
}

void Player::move(Point<float> p, TIME inow)
{
    //control->setPoint(p);
    //control->setUpdate();
    //control->setDelay(1);
    //move(inow);
    updated = inow;
    prev = point;
    point = p;
    //std::cout << point << " | " << prev << std::endl;
}

bool Player::getMove(Point<float>& p)
{
    p = point;
    return (point != prev && isAlive());
}

void Player::setX(float x)
{
    if (control == 0) return;
    float diff = control->getDelay() * speed;
    /* Moved left */
    if (point.getX() < x)
    {
        if (point.getX() + diff < x) point.setX(point.getX() + diff);
        else point.setX(x);
    } else {
        if (point.getX() - diff > x) point.setX(point.getX() - diff);
        else point.setX(x);
    }
}

void Player::setY(float y)
{
    if (control == 0) return;
    float diff = control->getDelay() * speed;
    /* Moved left */
    if (point.getY() < y)
    {
        if (point.getY() + diff < y) point.setY(point.getY() + diff);
        else point.setY(y);
    } else {
        if (point.getY() - diff > y) point.setY(point.getY() - diff);
        else point.setY(y);
    }
}

int Player::infect(int type)
{
    novirus();
    virus = now + Config::getInstance().CFG_VIRUSTIME;
    if(type==0) speed = Config::getInstance().CFG_SLOW_SPEED;
    if(type==1) speed = Config::getInstance().CFG_FAST_SPEED;
    if(type==2) virusMode = PLAYER_VIRUS(PUTPUT);
    if(type==3) virusMode = PLAYER_VIRUS(NOPUT);
    if(type==4) virusMode = PLAYER_VIRUS(UNLIMITED_BOMB);
    if(type==5) virusMode = PLAYER_VIRUS(UNLIMITED_FLAME);
    if(type==6) virusMode = PLAYER_VIRUS(UNEXPECTED_MONSTER);
    return type;
}

bool Player::isImmortal() const
{
    return spawned + Config::getInstance().CFG_IMMORTAL > now;
}

void Player::die()
{
    setLife(0);
    died = now;
}

void Player::win()
{
    life = -1024;
}

std::string Player::state()
{
    WAY w = Way(point, prev);
    // std::cout << "state " << point << " | " << prev << " | " << w << std::endl;
    
    if (control == 0 && now - updated > 100) w = STILL;
    
    if(prevWay != w)
    {
        change = now;
        prevWay = w;
        step = 0;
    }
    std::string ret = skin;
    unsigned int l = skin.length();
    if(w == STILL) ret.append("_d.png");
    if(w == RIGHT) ret.append("_r.png");
    if(w == LEFT) ret.append("_r.png#v");
    if(w == UP) ret.append("_u.png");
    if(w == DOWN) ret.append("_d.png");

    if(ret==".png") return ret;
    if(now - change > 25/speed)
    {
        step++;
        change = now;
    }
    if(w == LEFT || w == RIGHT)
    {
        if(step == 1) ret.insert(l+2,"_2");
        if(step == 3) ret.insert(l+2,"_3");
        if(step>3) step=0;
    }
    if(w == UP || w == DOWN)
    {
        if(step == 1) ret.insert(l+2,"_2");
        if(step == 3) ret.insert(l+2,"_2").append("#v");
        if(step > 3) step=0;
    }
    return ret;
}

std::string Player::getAura() const
{
    if (virus != 0 && virusMode != UNEXPECTED_MONSTER)
    {
        return "virus.png";
    }
    if (isImmortal())
    {
        return "immortal.png";
    }
    return "";
}

bool Player::getPut() const {
    if(life > 0)
    {
        if(bombs<=0 && virusMode != UNLIMITED_BOMB) return false;
        if(virusMode == PUTPUT) return true;
        if(virusMode == NOPUT) return false;
        return put;
    } else return false;
}

bool Player::getPutForce() const {
    if (control == 0) return false;
    return control->getPut();
}
