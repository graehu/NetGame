#include "header/entity.hpp"
#include <stdio.h>

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
//    mReceivedKey = 0;  ///pack
//    mEntities.clear();
    mPx =  0;     ///pack
    mPy =  0;     ///pack
    mVx =  1;     ///pack
    mVy =  1;     ///pack
    mType = eEntity;   ///pack
    //mSendKey = aSendKey;
//    mInitialised = false;
    mCommand = ' ';
    mPacket = 0;
}

void entity::readInitPacket(void)
{
    if(mPacket == 0)
    {
        /// do something more error relevant
        printf("entity: readInitPacket() error, packet null\n");
        return;
    }

    /// this packet contains
    ///
}
void entity::readUpdatePacket(void)
{
   /* if(mPacket == 0)
    {
        /// do something more error relevant
        printf("entity: readUpdatePacket() error, packet null\n");
        return;
    }

    if(!mInitialised)
    {
        printf("entity: readUpdatePacket() error, receiving update before initialised\n");
        return;
    }

    /// this packet contains

    ///TODO this read function is all wrong probably...

    ///mPacket->readUShort(0);     /// mId, array access on the otherside
    mPx = mPacket->readUInteger(0);   /// x pos
    mPy = mPacket->readUInteger(4);   /// y pos
    mVx = mPacket->readUInteger(8);  /// x velocity
    mVy = mPacket->readUInteger(12);  /// y velocity*/
}

void entity::writeInitPacket(void)
{

	if(mPacket == 0)
    {
        /// do something more error relevant
        printf("entity: writeInitPacket() error, packet null\n");
        return;
    }

	/*mPacket->pushData((unsigned short)mSendKey); /// me attempting to access them.
	mPacket->pushData((unsigned short)mReceivedKey); /// them attempting to access me.
    mPacket->pushData((unsigned short)eEntity);*/
    return;

}

void entity::writeUpdatePacket(void)
{

/*
	/* /// old method
        unsigned char packet[18];///some size, so far there are only 18 bytes in the update packet for an entity

        mPacman.writeShort(packet, mId);
        mPacman.writeInteger(packet + 2, mPosition.x);
        mPacman.writeInteger(packet + 6, mPosition.y);
        mPacman.writeInteger(packet + 10, mVelocity.x);
        mPacman.writeInteger(packet + 14, mVelocity.y);
	*/
	/*

    if(mPacket == 0)
    {
        printf("entity: writeUpdatePacket() error, packet pointer null\n");
        return;
    }

    if(!mInitialised)
    {
        printf("entity: writeUpdatePacket() error, writing update before initialised\n");
        return;
    }


    mPacket->pushData(mReceivedKey);
    mPacket->pushData(mPx);
    mPacket->pushData(mPy);
    mPacket->pushData(mVx);
    mPacket->pushData(mVy);

*/

}

void entity::move(void)
{
    if((mCommand&1) > 0) mPy-=1;
    if((mCommand&2) > 0) mPx-=1;
    if((mCommand&4) > 0) mPy+=1;
    if((mCommand&8) > 0) mPx+=1;

}



void entity::setPacket(packet* aPacket)
{
    mPacket = aPacket;
}
