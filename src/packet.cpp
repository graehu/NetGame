#include "header/packet.h"
#include <cstdio>
#include <cstdarg>

packet::packet()
{
	mAlloc = 0;
	mPacking = false;
	mHeaderSize = mEnd = 16;
    mData = new unsigned char[mHeaderSize];
	mTypeIttorator = 0;
    mCurrentDefID = 0;
    mSizeAccumulator = 0;
    mAnalysed = false;

}

packet::~packet()
{ ///this may require more later.
    if(mData != 0)
	delete mData;
}


bool packet::beginPacketData(unsigned int aPacketDefID)
{
    if(aPacketDefID < mDefinitions->size())
    {
        assert(!mPacking);
        if(!mPacking)
        {
            mCurrentDefID = aPacketDefID;
            mPacking = true;
            safePushData((unsigned short)mCurrentDefID);
            return false;
        }
        else return true;
    }
    else
    {
        printf("packet: beginPacketData(), error aPacketDefID too large\n");
        return true;
    }
}

bool packet::endPacketData()
{
    if(mPacking)
    {
        if(mSizeAccumulator != mDefinitions->getDefSize(mCurrentDefID))
        {
            printf("packet: endPacketData(), warning packet size inadequate.\n");
            //assert(false);
        }
        mSizeAccumulator = 0;
        mTypeIttorator = 0;
        mCurrentDefID = 0;
        mPacking = false;
        return false;
    }
    else return true;
}


/*
void packet::pushPacket(unsigned int aPacketDefID, ...)
{
    if(aPacketDefID < mDefinitions->size())
        return;
    va_list listPointer;

    va_start(listPointer, mDefinitions->getNumOfMembsInDef(aPacketDefID));


}
*/

void packet::analysePacket(void)
{
    unsigned short offset = mHeaderSize;
    unsigned short secType = readUShort(offset);
    bool data = true;
    unsigned int i = 0;
    unsigned int ii = 0;

    if(secType < mDefinitions->size())
    {
        while(data)
        {
            while(mDefinitions->getMembTypeName(secType, ii) != 0)
            {

                if(i >= mSectionInfo.size())
                {
                    //printf("ok wtf\n");
                    mSectionInfo.push_back(std::vector<std::pair<char*, unsigned int> >());
                }

                if(ii >= mSectionInfo[i].size())
                {
                    mSectionInfo[i].push_back(std::pair<char*,unsigned short>(mDefinitions->getMembTypeName(secType, ii), offset));
                    //printf("ok wtf\n");
                }
                else
                {
                    mSectionInfo[i][ii].first = mDefinitions->getMembTypeName(secType, ii);
                    mSectionInfo[i][ii].second = offset;

                }

                offset += mDefinitions->getTypeSizeByName(mDefinitions->getMembTypeName(secType, ii));
                ii++;
            }
            secType = readUShort(offset);
            ii = 0;
            i++;
            if(secType == 0)
            {
                data = false;
            }
        }
        //i = 0;
        /*if(i >= mSectionInfo.size())
        {
            mSectionInfo.push_back(std::vector<std::pair<char*, unsigned int> >());
            mSectionInfo[i].push_back(std::pair<char*,unsigned int>());
            mSectionInfo[i][0].first = (char*)typeid(void).name();
            mSectionInfo[i][0].second = offset;
        }
        else
        {
            mSectionInfo[i][0].first = (char*)typeid(void).name();
            mSectionInfo[i][0].second = offset;
        }*/

    }
    else return; //brooooken

    mAnalysed = true;
}

unsigned int packet::getSectionType(unsigned int aSection)
{
    if(aSection < mSectionInfo.size() && aSection >= 0)
    {
        if(!mSectionInfo[aSection].empty())
        return readUShort(mSectionInfo[aSection][0].second);

        //printf("packet: getSectionType(), error section = %i, no such section\n", aSection);
        return 0;
    }
    else
    {
        //printf("packet: getSectionType(), error section = %i, no such section\n", aSection);
        return 0;
    }
}

unsigned int packet::getSectionStart(unsigned int aSection)
{
    if(aSection < mSectionInfo.size())
    {
        return mSectionInfo[aSection][0].second;
    }
}

bool packet::setAlloc(unsigned int aAlloc)
{

	if(aAlloc > mAlloc)
	{
		unsigned char* temp;
		temp = mData;
		mData = new unsigned char[aAlloc];
		if(temp != 0)
		{
            memcpy(&mData[0], temp, mEnd);
            delete temp;
		}
		mAlloc = aAlloc;

		return false;
	}
	return true;
}

bool packet::pushData(const unsigned char aData[], unsigned int aSize)
{
	if((mEnd + aSize) > mAlloc)
	{
		unsigned char* temp;
		temp = mData;
		mData = new unsigned char[(mEnd + aSize)*2];
		if(temp != 0)
		{
            memcpy(&mData[0], temp, mEnd);
            delete temp;
		}
		mAlloc = (mEnd + aSize)*2;
	}

	memcpy(&mData[mEnd], aData, aSize);
	mEnd += aSize;
	return false;
}

unsigned int packet::readUInteger(unsigned int offset)
{
    if(offset + sizeof(short)> mEnd)
    {
        printf("Packet: readUInteger(): offset too large.\n");
        return 0;
    }

    unsigned int number = (((unsigned int)mData[0+offset] << 24) | ((unsigned int)mData[1+offset] << 16) |
                           ((unsigned int)mData[2+offset] << 8)  | ((unsigned int)mData[3+offset]));
    return number;
}

int packet::readInteger(unsigned int offset)
{
    if(offset + sizeof(int) > mEnd)
    {
        printf("Packet: readInteger(): offset too large.\n");
        return 0;
    }

    int number = (((int)mData[0+offset] << 24) | ((int)mData[1+offset] << 16) |
                  ((int)mData[2+offset] << 8)  | ((int)mData[3+offset]));
    return number;
}

unsigned short packet::readUShort(unsigned int offset)
{

    if(offset + sizeof(short) > mEnd)
    {
        printf("Packet: readUShort(): offset too large.\n");
        return 0;
    }

    unsigned short number = (((unsigned short)mData[0+offset] << 8) |
                    ((unsigned short)mData[1+offset]));

    return number;
}

short packet::readShort(unsigned int offset)
{
    if(offset+sizeof(short) > mEnd)
    {
        printf("Packet: readShort(): offset too large.\n");
        return 0;
    }
    short number = (((short)mData[0+offset] << 8) |
                    ((short)mData[1+offset]));

    return number;
}


unsigned char packet::readUChar(unsigned int offset)
{

    if(offset + sizeof(unsigned char) > mEnd)
    {
        printf("Packet: readUChar(): offset too large.\n");
        return 0;
    }

    unsigned char character = ((unsigned char)mData[offset]);

    return character;
}

char packet::readChar(unsigned int offset)
{
    if(offset+sizeof(short) > mEnd)
    {
        printf("Packet: readChar(): offset too large.\n");
        return 0;
    }
    char character = ((char)mData[offset]);

    return character;
}
