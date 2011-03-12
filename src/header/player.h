#ifndef PLAYER_H
#define PLAYER_H

#include "entity.hpp"

class player : public entity
{
	public:

		player(){};
		~player(){};

	protected:

    unsigned char* updatePacket();
    unsigned char* initPacket();    /// override in defined, make sure to call baseclass version too

	private:

    char name[16];
    unsigned short strength;
    unsigned short range;
    unsigned short defence;
    unsigned short timestamp;
    unsigned short health;

};


#endif//PLAYER_H
