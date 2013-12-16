#include "animfile.h"

AnimFile::AnimFile(std::string filename)
{
    std::string wdir("img/");
    wdir.append(filename);
    std::ifstream f(wdir.c_str());
    while(f.good())
    {
        std::string a;
        TIME b;
        f >> a >> b;
        if(a != "")
        {
            for(std::string::iterator i = a.begin(); i != a.end(); ++i) if(*i==',') *i=' ';
            frames.push_back(Frame(a,b));
            printf("%s %d\n",a.c_str(),b);
        }
    }
    f.close();
}

std::string AnimFile::frame(TIME time)
{
    TIME curr = 0;
    for(std::vector<Frame>::iterator i = frames.begin(); i != frames.end(); ++i)
    {
        if(curr + i->time >= time) return i->obj;
        curr += i->time;
    }
    return "";
}
