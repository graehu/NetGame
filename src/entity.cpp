#include "header/entity.hpp"
//#include "header/packer.hpp"

///update packet layout
///{
/// id
/// posx
/// posy
/// velx
/// vely
///}

entity::entity()
{
    mId = 0;            ///don't need to pack
    mViewers.clear();  ///don't need to pack
    mPosition.x = 0;     ///pack
    mPosition.y = 0;     ///pack
    mVelocity.x = 0;     ///pack
    mVelocity.y = 0;     ///pack
    mType = eEntity;   ///pack
}

unsigned char* entity::updatePacket()
{
    ///may need to think of some seriously... better packing algorithms.

	/* /// old method
    unsigned char packet[18];///some size, so far there are only 18 bytes in the update packet for an entity

    mPacman.writeShort(packet, mId);
    mPacman.writeInteger(packet + 2, mPosition.x);
    mPacman.writeInteger(packet + 6, mPosition.y);
    mPacman.writeInteger(packet + 10, mVelocity.x);
    mPacman.writeInteger(packet + 14, mVelocity.y);
	*/

	mPacman.pushData(mId);
    mPacman.pushData(mPosition.x);
    mPacman.pushData(mPosition.y);
    mPacman.pushData(mVelocity.x);
    mPacman.pushData(mVelocity.y);

    return mPacman.getData();
}

unsigned char* entity::initPacket()
{
	/* ///old method
    unsigned char packet[2];
	mPacman.writeShort(packet, 0); ///i.e. eEntity
	//this also needs to send an array position.
	*/
    return 0;
}


/*

	an object wihch stores the key of itself on the otherside
	each object has an array

*/





