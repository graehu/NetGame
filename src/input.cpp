#include "header/input.h"

input::input()
{
}

input::~input()
{
}



bool input(bool* keys)
{
    for(int i = 0; i < eKeysSize; i++)
        keys[i] = false;

    char* internalKeys = (char*)SDL_GetKeyState(NULL);
    
    if (internalKeys['w'])
      keys[eUp] = true;
    if (internalKeys['a'])
      keys[eLeft] = true;
    if (internalKeys['s'])
      keys[eDown] = true;
    if (internalKeys['d'])
      keys[eRight] = true;

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                return true;
            }
        }
    }
    return false;
}



void input::init(void)
{
}
void input::update(void)
{

    for(unsigned int i = 0; i < eTotalKeys; i++)
        keys[i] = false;
  
    char* internalKeys = (char*)SDL_GetKeyState(NULL);

    //    if(

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_QUIT:
            {
                return true;
            }
        }
    }
    return false;


}
