#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL/SDL.h>

#include "header/net.h"

//#define SHOW_ACKS

using namespace std;
using namespace net;

const int ServerPort = 8000;
const int ClientPort = 8001;
const unsigned short ProtocolId = 0xf00d;//80;
const float DeltaTime = 1.0f/60.0f;
const float SendRate = 1.0f / 30.0f;
const float TimeOut = 100.0f;
const int PacketSize = 256;

bool input(bool* keys);

enum Keys
{
    eUp,
    eDown,
    eLeft,
    eRight,
    eKeysSize
};


int main(int argc, char * argv[])
{
    network myNet(0xF00D, TimeOut);
    bool keys[4];

    if(!(argc >= 2))
    {
        if(myNet.init(eServer, 8000) == 1)
        {
            bool unbound = true;
            int i = 1;
            while(unbound)
            {
                if(myNet.init(eClient, 8000+i) == 0)
                {
                    unbound = false;
                }
                i++;
            }
        }
    }
    else
    {
        int port = 0;
        if(sscanf(argv[1],"%i",&port))
            myNet.init(eClient, port);
        else
            return 0;
    }

    while(true)
    {
        myNet.update(DeltaTime);
        waitsecs(DeltaTime);
        if(input(keys) == true) return 0;
        if(myNet.getType() == eClient)
        {
            if(input(keys) == true) return 0;
            if(keys[(int)eUp] == true)
            {
                printf("up\n");
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|1);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-1));}
            if(keys[(int)eLeft] == true)
            {
                printf("left\n");
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|2);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-2));}
            if(keys[(int)eDown] == true)
            {
                printf("down\n");
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|4);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-4));}
            if(keys[(int)eRight] == true)
            {
                printf("right\n");
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|8);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-8));}
        }
        myNet.draw();
    } 
    return 0;
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
