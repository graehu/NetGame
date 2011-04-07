#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"

class player : public entity
{
	public:

		player();
		~player(){};

    void readInitPacket(void);
    void readUpdatePacket(void);

    void writeInitPacket();    /// override in defined, make sure to call baseclass version too
    void writeUpdatePacket();

	protected:

	private:

    char mName[16];
    unsigned short mStrength;
    unsigned short mDefence;
    unsigned short mRange;
    unsigned short mHealth;


};


#endif//PLAYER_H
