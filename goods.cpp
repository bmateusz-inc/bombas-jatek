#include "goods.h"

std::string Goods::state()
{
    switch(type)
    {
        case FLAME: return "cucc_flame.png"; break;
        case BOMB:  return "cucc_bomb.png"; break;
        case VIRUS: return "cucc_virus.png"; break;
    }
    return "";
}

PLAYER_VIRUS Goods::getRandVirus()
{
    return PLAYER_VIRUS(rand()%7);
}

int Goods::effect(Player *p, std::string &text)
{
    int infection;
    if (type == VIRUS) infection = getRandVirus();
    else infection = type;
    text = effect(p,infection);
    return infection;
}

std::string Goods::effect(Player* p, int infection)
{
    std::string text;
    if (infection == FLAME)
    {
        p->incSize();
        text = "+1 Tűz";
    }
    else if (infection == BOMB)
    {
        p->incBombs();
        text = "+1 Bomba";
    }
    else
    {
        switch(infection)
        {
            case 0: text = "    Lassú!"; break;
            case 1: text = "    Gyors!"; break;
            case 2: text = "   Beragadt!"; break;
            case 3: text = "   0 bomba!"; break;
            case 4: text = "Végtelen bomba!"; break;
            case 5: text = " Végtelen tűz!"; break;
            case 6: text = "    Szörny!"; break;
        }
        p->infect(infection);
    }
    return text;
}
