#include "header/game.h"

const float TimeOut = 100.0f;
bool input(bool* keys);

enum Keys
  {
    eUp,
    eDown,
    eLeft,
    eRight,
    eKeysSize
  };


game::game()
{
  mRunning = false;
}
game::~game()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool game::init()
{

  mNet = new network(0xF00D, TimeOut); ///time out time + protocol id.
  if(mNet->init(true, 8000) == 1)
    {
      bool unbound = true;
      int i = 1;
      while(unbound)
	{
	  if(mNet->init(false, 8000+i) == 0)
	    {
	      unbound = false;
	    }
	  i++;
	}
    }
  mRunning = true;
  return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool game::run()
{
  bool keys[4];

  while(mRunning)
    {
      mNet->update(DeltaTime);
      waitsecs(DeltaTime);
      if(input(keys) == true) return 0;
      if(mNet->getType() == eClient)
        {
	  if(input(keys) == true) return 0;
	  if(keys[(int)eUp] == true)
            {
	      printf("up\n");
	      mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()|1);
            }
	  else{mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()&(255-1));}
	  if(keys[(int)eLeft] == true)
            {
	      printf("left\n");
	      mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()|2);
            }
	  else{mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()&(255-2));}
	  if(keys[(int)eDown] == true)
            {
	      printf("down\n");
	      mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()|4);
            }
	  else{mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()&(255-4));}
	  if(keys[(int)eRight] == true)
            {
	      printf("right\n");
	      mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()|8);
            }
	  else{mNet->getEntity(0)->setCommands(mNet->getEntity(0)->getCommands()&(255-8));}
        }
      mNet->draw();
    } 
  return 0;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
