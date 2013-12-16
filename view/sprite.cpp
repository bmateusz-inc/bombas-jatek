#include "sprite.h"

bool Sprite::load(std::string fileName)
{
    fn = fileName;
    std::string wdir("img/");
    //std::string wdir("img_low/");
    wdir.append(fileName);

    std::vector<unsigned char> png;
    std::vector<unsigned char> image; //the raw pixels
    lodepng::State state; //optionally customize this one

    lodepng::load_file(png, wdir); //load the image file with given filename
    unsigned error = lodepng::decode(image, w, h, state, png);

    if(error != 0)
    {
        printf("error %ud: %s\n",error,lodepng_error_text(error));
        return false;
    }
    else
    {

        // Check that the image's width is a power of 2
        if ( (w & (w - 1)) != 0 ) {
            printf("warning: %s width is not a power of 2\n", fileName.c_str());
        }

        // Also check if the height is a power of 2
        if ( (h & (h - 1)) != 0 ) {
            printf("warning: %s height is not a power of 2\n", fileName.c_str());
        }

        // get the number of channels
        LodePNGColorType colors = state.info_png.color.colortype;
        if (colors == LCT_RGBA) { texture_format = GL_RGBA; nColors = 4; }
        else if (colors == LCT_RGB) { texture_format = GL_RGBA; nColors = 3; }
        else printf("warning: %s is not truecolor..  this will probably break\n", fileName.c_str());

        // Have OpenGL generate a texture object handle for us
        glGenTextures(1, &texture);

        // Bind the texture object
        glBindTexture(GL_TEXTURE_2D, texture );

        // Set the texture's stretching properties
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);

        // Edit the texture object's image data using the information SDL_Surface gives us
        glTexImage2D(GL_TEXTURE_2D, 0, nColors, w, h, 0, texture_format, GL_UNSIGNED_BYTE, &image.front());

        //glGenerateMipmap(GL_TEXTURE_2D);
        printf("loaded %s\n", fileName.c_str());
        return true;
    }
}

Sprite::~Sprite()
{
    // Free the SDL_Surface only if it was successfully created
    /*if ( surface ) {
        SDL_FreeSurface( surface );
    }*/
    glDeleteTextures( 1, &texture );
    //printf("free %s\n", fn.c_str());
}


void Sprite::draw(float x, float y, float w, float h, std::string param)
{
    glBindTexture( GL_TEXTURE_2D, texture );

    GLfloat vertex[]   = { x,   y,   0,
                           x+w, y,   0,
                           x+w, y+h, 0,
                           x,   y+h, 0 };

    GLfloat texcoord[] = { 0, 0,
                           1, 0,
                           1, 1,
                           0, 1 };
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


    /*
     * Immediate Mode
    glBegin( GL_QUADS );
        //Bottom-left vertex (corner)
        glTexCoord2i( 0, 0 );
        glVertex3f( x, y, 0 );

        //Bottom-right vertex (corner)
        glTexCoord2i( 1, 0 );
        glVertex3f( x + w, y, 0 );

        //Top-right vertex (corner)
        glTexCoord2i( 1, 1 );
        glVertex3f( x + w, y + h, 0 );

        //Top-left vertex (corner)
        glTexCoord2i( 0, 1 );
        glVertex3f( x, y + h, 0 );
    glEnd();
    */
}
