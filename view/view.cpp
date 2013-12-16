#include "view.h"
#include "config.h"

View::View()
{
    if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) != 0 ) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        return;
    }

    SDL_ShowCursor(SDL_DISABLE);

    //int sw=0; int sh=0;
    int sw=800; int sh=600;
    keep_ratio = true;
    screenshot = false;
    toggle = false;
    screen = 0;
    now = prev = 0;

    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    const SDL_VideoInfo* desktop_info = SDL_GetVideoInfo();
    desktop = Point<int>(desktop_info->current_w, desktop_info->current_h);
    printf("Desktop: %d x %d\n", desktop.getX(), desktop.getY());

    resize_event(sw,sh);

    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
    //glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
    //glDisable(GL_SMOOTH);

    //glShadeModel(GL_SMOOTH);
    //glDisable(GL_CULL_FACE);
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glDepthFunc(GL_NEVER);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0f, 640, 480, 0.0f, -1.0f, 1.0f);

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void View::frameBegin()
{
    prev = now;
    setNow();
}

void View::frameEnd()
{
    TIME next_time;
    TIME end;
    next_time = now + TICK_INTERVAL;
    end = GetTicks();
    if (Config::getInstance().CFG_TEST_FPS == 0)
        if(end < next_time) Delay(next_time - end);
    if (Config::getInstance().CFG_TEST_FPS == 1)
        Delay(100);
    if (Config::getInstance().CFG_TEST_FPS == 2)
        Delay(200);
    if (Config::getInstance().CFG_TEST_FPS == 3)
        Delay(1000);
}

TIME View::frameDelay()
{
    int delay = now-prev;
    if (delay > 200) return 200;
    return delay;
}

void View::toggleFullScreen()
{
    /*int w,h;
    toggle=false;
    if(screen->flags & SDL_FULLSCREEN) { w=800; h=600; }
    else { w=h=0; }
    printf("toggle %d\n",w);
    resize_event(w,h);*/
}

void View::resize_event(int &w, int &h)
{
    Uint32 flags;
    if(screen==0) flags = SDL_OPENGL | SDL_RESIZABLE;
    else flags = screen->flags;

    if(w==0 && h==0)
    {
        //flags |= SDL_FULLSCREEN;
        //flags &= ~SDL_RESIZABLE;
        w = desktop.getX();
        h = desktop.getY();
    }
    else
    {
        //flags &= ~SDL_FULLSCREEN;
        //flags |= SDL_RESIZABLE;
    }
    screen = SDL_SetVideoMode( w, h, 32, flags );
    int x = 0;
    int y = 0;
    if(keep_ratio)
    {
        float ratio = (float)w / (float)h;
        float normal = 4.f / 3.f;
        if(ratio > normal)
        {
            float diff = w - w / ratio * normal;
            x = diff / 2;
            w -= diff;
        }
        if(ratio < normal)
        {
            float diff = h - h * ratio / normal;
            y = diff / 2;
            h -= diff;
        }
    }
    glViewport(x, y, w, h);
}

View::~View()
{
    //printf("unload view\n");
    
    draw(10,50,16,32,"font.txh#Viszlát!");
    swap();
    
    for(std::map<std::string, Object*>::iterator i = graphs.begin(); i != graphs.end(); ++i)
    {
        //printf("unload %s\n", i->first.c_str());
        delete i->second;
    }

    if(screen)
    {
        //printf("unload surface\n");
        SDL_FreeSurface( screen );
    }
    //printf("unload ogl\n");
    TerminateOpenGL();
}

void View::swap()
{
    //SDL_GL_SwapBuffers();
    if(screenshot) Screenshot_do();
    SwapBuffers();
    glClear(GL_COLOR_BUFFER_BIT);
    if(toggle) toggleFullScreen();
}

bool View::loadPackage(std::string fileName)
{
    std::ifstream f(fileName.c_str());
    if(f.fail()) return false;
    
    int numberOfFiles;
    int n=0;
    f >> numberOfFiles;
    swap();
    load("loading.png");
    load("border.png");

    while(f.good())
    {
        std::string a;
        f >> a;
        if(a != "")
        {
            if(n%2) {
                draw(10,10,128,32,"loading.png");
                int loaded = 620.0 * ((float)n/numberOfFiles);
                printf("%d",loaded);
                draw(10,50,loaded,32,"border.png");
                swap();
            }
            n++;

            if(a.rfind(".png") != std::string::npos) load(a);
            else if(a.rfind(".anm") != std::string::npos) loadAnim(a);
            else if(a.rfind(".mlt") != std::string::npos) loadMulti(a);
            else if(a.rfind(".txh") != std::string::npos) loadText(a);
        }
    }

    return true;
}

bool View::load(std::string fileName)
{
    //printf("load %s\n", fileName.c_str());

    if(graphs.count(fileName) > 0)
    {
        graphs.erase(fileName);
    }

    Sprite* s = new Sprite(fileName);
    graphs.insert( std::pair<std::string, Sprite*>(fileName, s) );
    return true;
}

bool View::loadMulti(std::string fileName)
{
    //printf("load %s\n", fileName.c_str());

    if(graphs.count(fileName) > 0)
    {
        graphs.erase(fileName);
    }

    Multi* m = new Multi(fileName, &graphs);
    graphs.insert( std::pair<std::string, Multi*>(fileName, m) );
    return true;
}

bool View::loadText(std::string fileName)
{
    //printf("load %s\n", fileName.c_str());

    if(graphs.count(fileName) > 0)
    {
        graphs.erase(fileName);
    }

    Text* t = new Text(fileName, &graphs);
    graphs.insert( std::pair<std::string, Text*>(fileName, t) );
    return true;
}

std::string View::getRes(std::string str)
{
    size_t p = str.find("#");
    return str.substr(0,p);
}
std::string View::getParam(std::string str)
{
    size_t p = str.find("#");
    if(p == std::string::npos) return "";
    else return str.substr(p+1);
}

void View::draw(Rect r, std::string res)
{
    draw(r.x,r.y,r.w,r.h,res);
}

void View::draw(float x, float y, float w, float h, std::string res)
{
    if(res.empty()) return;
    if(graphs.count(getRes(res)) < 1) { printf("BAJ %s\n",res.c_str()); return; }
    graphs[getRes(res)]->draw(x,y,w,h,getParam(res));
}

void View::loadAnim(std::string fileName)
{
    if(graphs.count(fileName) > 0)
    {
        graphs.erase(fileName);
    }
    
    anims.insert( std::pair<std::string, AnimFile>(fileName, AnimFile(fileName)) );
    AnimObject *a = new AnimObject(&anim,&anims,fileName,&now);
    graphs.insert(std::pair<std::string, AnimObject*>(fileName, a));
}

void View::drawAnim()
{
    for(std::deque<Animation>::iterator i = anim.begin(); i != anim.end();)
    {
        std::string frame = i->draw(now);
        if(frame != "")
        {
            draw(i->getRect(), frame);
            ++i;
        }
        else
        {
            i = anim.erase(i);
        }
    }
}

void View::stopAnim()
{
    anim.clear();
}

void View::Screenshot()
{
    screenshot = true;
}
void View::Screenshot_do()
{
    screenshot = false;
    char cFileName[64];
    FILE *fScreenshot;
    int w = getW();
    int h = getH();
    int nSize = w * h * 4;

    GLubyte *pixels = new GLubyte [nSize];
    if (pixels == NULL) return;

    int nShot = 0;
    while (nShot < 64)
    {
        sprintf(cFileName,"screenshot_%d.png",nShot);
        fScreenshot = fopen(cFileName,"rb");
        if (fScreenshot == NULL) break;
        else fclose(fScreenshot);

        ++nShot;
        if (nShot > 63)
        {
            printf("Screenshot limit of 64 reached. Remove some shots if you want to take more.\n");
            return;
        }
    }

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

    for(int i=0; i<h/2; ++i)
    {
        int k = h-i-1;
        for(int j=0; j<w*4; ++j)
        {
            GLubyte t;
            t = pixels[i*w*4 + j];
            pixels[i*w*4 + j] = pixels[k*w*4 + j];
            pixels[k*w*4 + j] = t;
        }
    }

    unsigned error = lodepng::encode(cFileName, pixels, w, h);

    if(error) printf("encoder error %d: %s\n", error, lodepng_error_text(error));
    else printf("Screenshot taken.\n");
}
