#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "packet.h"


class entity
{
	public:
		entity();
		~entity();


    void readInitPacket(void);
    void readUpdatePacket(void);

    void writeInitPacket(void);    /// override in defined, make sure to call baseclass version too
    void writeUpdatePacket(void);  /// override in defined, make sure to call baseclass version too
    entityType getType(void){return mType;}
    int getXPos(void){return mPx;}
    int getYPos(void){return mPy;}
    int getXVelocity(void){return mVx;}
    int getYVelocity(void){return mVy;}
    unsigned char getCommands(void){return mCommand;}

    void setXPos(int x){mPx = x;}
    void setYPos(int y){mPy = y;}
    void setXVelocity(int x){mVx = x;}
    void setYVelocity(int y){mVy = y;}
    void setCommands(unsigned char command){mCommand = command;}

    void move(void);




    void setPacket(packet* aPacket);
    //unsigned short getReceivedKey(void){return mReceivedKey;}   /// return his position in some array

	protected:

    packet* mPacket;
    entityType mType;

	private:

	//bool mInitialised;

    //unsigned short mSendKey; /// this array
    //unsigned short mReceivedKey; /// their array
    //std::vector<std::pair<entityType*,unsigned short*> > mEntities; ///things this dude knows of.
    int mPx, mPy, mVx, mVy; ///positions and velocities
    int mNewPx, mNewPy, mNewVx, mNewVy;
    unsigned char mCommand;

};


#endif // ENTITY_H

