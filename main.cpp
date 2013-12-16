#include "logger.h"
#include "game.h"

/**
 * Main entry point of the program
 * @param argc number of arguments
 * @param argv string array of arguments
 * @return result of running
 */
#ifdef __WIN32

#include <windows.h>
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR arg, INT)
{
    // TODO generate args
    int returnVal = main(0,0); //main(__argc, __argv);
    return returnVal;
}
#endif

int main(int argc, char* argv[])
{
    Logger::getInstance().error("Start");
    //Logger::getInstance().error(argv[0]);
    Game g;
    printf("Exit\n");
    return 0;
}
