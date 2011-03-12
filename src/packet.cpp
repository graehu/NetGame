#include "header/packet.h"

packet::packet()
{
	mAlloc = 0;
	mSize = 0;
	mData = 0;
}

packet::~packet()
{ //this may require more later.
	delete mData;
}
///this function is problematic for some reason
bool packet::setAlloc(unsigned int aAlloc)
{
	if(aAlloc > mAlloc)
	{
		unsigned char* temp;
		temp = mData;
		mData = new unsigned char[aAlloc];
		memcpy(&mData, temp, mSize);
		delete temp;
		mAlloc = aAlloc;
		return false;
	}
	return true;
}

bool packet::pushData(const unsigned char aData[], unsigned int aSize)
{
	if((mSize + aSize) > mAlloc)
	{
		unsigned char* temp;
		temp = mData;
		mData = new unsigned char[(mSize + aSize)*2];
		memcpy(&mData[0], temp, mSize);
		delete temp;
		mAlloc = (mSize + aSize)*2;
	}

	memcpy(&mData[mSize], aData, aSize);
	mSize += aSize;
	return false;
}


void packet::writeInteger(unsigned char * aData, unsigned int aValue)
{
    aData[0] = (unsigned char)(aValue >> 24);
    aData[1] = (unsigned char)((aValue >> 16) & 0xFF);
    aData[2] = (unsigned char)((aValue >> 8) & 0xFF);
    aData[3] = (unsigned char)(aValue & 0xFF);
}

void packet::readInteger(const unsigned char * aData, unsigned int & aValue)
{
    aValue = (((unsigned int)aData[0] << 24) | ((unsigned int)aData[1] << 16) |
              ((unsigned int)aData[2] << 8)  | ((unsigned int)aData[3]));
}

void packet::writeShort(unsigned char * aData, unsigned short aValue)
{
    aData[0] = (unsigned char)((aValue >> 8) & 0xFF);
    aData[1] = (unsigned char) (aValue & 0xFF);
}

void packet::readShort(const unsigned char * aData, unsigned short & aValue)
{
    aValue = (((unsigned int)aData[0] << 8) |
			  ((unsigned int)aData[1]));
}
