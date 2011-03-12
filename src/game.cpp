#include "header/game.h"

game::game()
{
    running = false;
}
game::~game()
{

}

bool game::init() //not sure if this should take a parameter yet
{
    running = true;

    //net.init() //this'll be replaced with server init or client init

    return false;
}


bool game::run()
{
    while(running)
    {
        //do logic and stuff...


    }

    return false;
}
