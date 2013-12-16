#include "multi.h"

bool Multi::load(std::string fileName)
{
    std::string wdir("img/");
    wdir.append(fileName);
    std::ifstream f(wdir.c_str());
    f >> name >> sx >> sy;
    f.close();
    sx = 1/sx;
    sy = 1/sy;
    printf("%s: %f %f\n",name.c_str(),sx,sy);
    return true;
}

void Multi::draw(float x, float y, float w, float h, std::string param)
{
    int ix=0;
    int iy=0;

    std::stringstream s(param);
    s >> ix;
    if (s.peek() == ',') s.ignore();
    s >> iy;

    ((*graphs)[name])->bind();

    GLfloat vertex[]   = { x,   y,   0,
                           x+w, y,   0,
                           x+w, y+h, 0,
                           x,   y+h, 0 };

    GLfloat texcoord[] = { sx*ix,     sy*iy,
                           sx*(ix+1), sy*iy,
                           sx*(ix+1), sy*(iy+1),
                           sx*ix,     sy*(iy+1) };
    if(param.find("h") != std::string::npos) {
        for(int i=0; i<2; ++i)
        {
            GLfloat t;
            t = texcoord[i+4];
            texcoord[i+4] = texcoord[i];
            texcoord[i] = t;
        }
    }
    if(param.find("v") != std::string::npos) {

        for(int i=0; i<2; ++i)
        {
            GLfloat t;
            t = texcoord[i+2];
            texcoord[i+2] = texcoord[i];
            texcoord[i] = t;
        }
        for(int i=4; i<6; ++i)
        {
            GLfloat t;
            t = texcoord[i+2];
            texcoord[i+2] = texcoord[i];
            texcoord[i] = t;
        }
    }
    if(param.find("r") != std::string::npos) {
        int t1=texcoord[0], t2=texcoord[1];
        for(int i=0; i<3; ++i)
        {
            texcoord[i*2] = texcoord[i*2+2];
            texcoord[i*2+1] = texcoord[i*2+3];
        }
        texcoord[6]=t1; texcoord[7]=t2;
    }
    if(param.find("l") != std::string::npos) {
        int t1=texcoord[6], t2=texcoord[7];
        for(int i=2; i<=0; --i)
        {
            texcoord[i*2+2] = texcoord[i*2];
            texcoord[i*2+3] = texcoord[i*2+1];
        }
        texcoord[0]=t1; texcoord[1]=t2;
    }

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertex);
    glTexCoordPointer(2, GL_FLOAT, 0, texcoord);

    glDrawArrays(GL_QUADS, 0, 4);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}
