#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "packet.h"

struct coords
{
    int x;
    int y;
};

enum entityType
{
    eEntity = 0,
    eBot,
    ePlayer
};



class entity
{
	public:
		entity();
		~entity();

    unsigned char* updatePacket();  /// override in defined, make sure to call baseclass version too
    unsigned char* initPacket();    /// override in defined, make sure to call baseclass version too
    unsigned short getId(void){return mId;}   /// return his position in some array

	protected:
	private:

    unsigned short mId;
    std::vector<unsigned short> mViewers; /// all the people I should be sent to. i.e. who can see me.
    coords mPosition;
    coords mVelocity;
    entityType mType;
	packet mPacman;

};


#endif // ENTITY_H

