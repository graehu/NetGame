#ifndef INPUT_H
#define INPUT_H

#include <SDL/SDL.h>

enum gameKey
  {
    eUp = 0,
    eDown,
    eLeft,
    eRight,
    eTotalKeys
  };

class input
{
 public:
  input();
  ~input();
 protected:

  void init(void);
  void getKeysPressed(bool* aKeys){aKeys = mKeys;} //if this returns false then halt the game.
  unsigned int getTotalGameKeys(void){return (unsigned int)eTotalKeys;}
  bool isClosed(void){return mClosed;}
  void update(void);

 private:

  bool mKeys[eTotalKeys];
  bool mClosed; //if the sdl quit is true.

};


#endif//INPUT_H
