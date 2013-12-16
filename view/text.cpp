#include "text.h"

bool Text::load(std::string fileName)
{
    std::string wdir("img/");
    wdir.append(fileName);
    std::ifstream f(wdir.c_str());
    f >> name;
    f >> m;
    f.close();
    return true;
}

void Text::draw(float x, float y, float w, float h, std::string param)
{
    int offset_x = 0, offset_y = 0;
    std::stringstream t(param);
    std::vector<Utf8> p;
    while(t.good())
    {
        Utf8 u;
        t >> u;
        //std::cout << "." << u;
        if(u.notNull()) p.push_back(u);
    }
    //std::cout<<"\n";
    //for(std::string::iterator i = param.begin(); i != param.end(); ++i)
    for(std::vector<Utf8>::iterator i = p.begin(); i != p.end(); ++i)
    {
        if(i->isBr())
        {
            offset_x = 0;
            ++offset_y;
        }
        if(i->notSpace())
        {
            Point<int> place = m.find(*i);
            std::stringstream s;
            s << place.getX() << " " << place.getY();
            //std::cout << "send: " << s.str() << "\n";
            ((*graphs)[name])->draw(x+offset_x*w, y+offset_y*h, w, h, s.str());
        }
        ++offset_x;
    }
}
