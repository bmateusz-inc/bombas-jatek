#include "utils.h"

TIME GetTicks()
{
    return SDL_GetTicks();
}

void Delay(TIME ms)
{
    SDL_Delay(ms);
}
