#ifndef PACKET_H
#define PACKET_H

#include "packetDef.h"
#include <cstring>
#include <cstdio>
//#include <iostream> //noooobish
#include <vector>

enum entityType
{
    eEntity = 0,
    ePlayer,
    eBot,
    eTotalEntities
};


class packet
{
	public:
	packet();
    ~packet();

    /// Packet data related functions
    //bool pushPacket(unsigned int aPacketDefID, ...);
	bool pushData(const unsigned char aData[], unsigned int aSize);
	bool clearPacket(void){mEnd = mHeaderSize; mAnalysed = false; return false;}
	bool setAlloc(unsigned int aAlloc);
	void setEnd(unsigned int aEnd){if(aEnd >! mAlloc && aEnd > mHeaderSize)mEnd = aEnd;}
	unsigned int getAlloc(void) {return mAlloc;}
	unsigned int getEnd(void){return mEnd;}
	unsigned char* getData(void){return mData;}
	unsigned short getType(void);

    /// header realated functionality.
	unsigned int getHeaderSize(void){return mHeaderSize;}
	unsigned short readProtocolId(void){return readUShort(0);}
    unsigned short readKey(void){return readUShort(2);}
    unsigned int readSequence(void){return readUInteger(4);}
    unsigned int readAck(void){return readUInteger(8);}
    unsigned int readAckBits(void){return readUInteger(12);}

    void writeProtocolId(unsigned short aId){writeData(aId,0);}
    void writeKey(unsigned short aKey){writeData(aKey,2);}
    void writeSequence(unsigned int aSeq){writeData(aSeq,4);}
    void writeAck(unsigned int aAck){writeData(aAck,8);}
    void writeAckBits(unsigned int aAckBits){writeData(aAckBits,12);}
    /// header realated functionality.

    void setDefinitions(packetDef* aDefines){mDefinitions = aDefines;} ///this est bad.
    packetDef* getDefinitions(void){return mDefinitions;}

    void analysePacket(void);
    unsigned int getSectionType(unsigned int aSection);
    unsigned int getSectionStart(unsigned int aSection);

    /// attempt to make a generic data reader, failed.
    /*template <typename T>
    void readData(unsigned int aOffset, T &aData)
    {
        //unsigned char temp[sizeof(aData)] =  {0};
        T temp = 0;
        for(int i = 0; i < sizeof(aData)-1; i++)
            temp = ((mData[aOffset+i] << ((sizeof(aData)-1)*8 -(i*8))) | (mData[aOffset+(sizeof(aData)-1)]));
            ((t)mData[aOffset])

            printf("hmmm: %i\n", temp);

        //aData = (T)temp;
    }*/

	/// stupid template functions can't be declared in one file and defined in another.
	template <typename T>
	bool pushData(T data)
	{
	    if(!mPacking){
		unsigned int size = sizeof(data);
		if((mEnd + size) > mAlloc)
		{
			unsigned char* temp;
			temp = mData;
			mData = new unsigned char[(mEnd + size)*2];
            if(temp != 0)
            {
                memcpy(&mData[0], temp, mEnd);
                delete temp;
            }
			mAlloc = (mEnd + size)*2;
		}

		writeData(data,mEnd);

		mEnd += size;
		return false;}else return true;
	};

    ///bad generic data writer.
	template <typename T>
    bool writeData(T data, unsigned int offset)
	{
		unsigned int size = sizeof(data);

		if((offset + size) > mAlloc)
		{
		    printf("Packet: writeData() error, offset too large.\n");
		    return true;
		}

		for(unsigned int i = 0; i < size; i++)
		{
			if (i > 0)
				mData[offset+i] = (data >> (size-(i+1))*8);
			else
				mData[offset+i] = ((data >> (size-(i+1))*8) & 0xFF);
		}
		return false;
	}

    bool beginPacketData(unsigned int aPacketDefID);
  	template <typename T>
	bool safePushData(T data)
	{
	    if(mPacking)
	    {
            if(typeid(T).name() == mDefinitions->getMembTypeName(mCurrentDefID, mTypeIttorator))
            {
                unsigned int size = sizeof(data);
                if((mEnd + size) > mAlloc)
                {
                    unsigned char* temp;
                    temp = mData;
                    mData = new unsigned char[(mEnd + size)*2];
                    if(temp != 0)
                    {
                        memcpy(&mData[0], temp, mEnd);
                        delete temp;
                    }
                    mAlloc = (mEnd + size)*2;
                }
                //printf("i'm pushing, right?\n");
                writeData(data,mEnd);

                mSizeAccumulator += size;
                mEnd += size;
                mTypeIttorator++;
                return false;
            }
            else
            {
                printf("packet: safePushData(), error expected:%s passed:%s\n",
                       (const char*)mDefinitions->getMembTypeName(mCurrentDefID, mTypeIttorator),
                       typeid(T).name());
                return true;
            }
		}
		else
		{
		    printf("packet: safePushDate(), error haven't begun packet\n");
            return true;
		}
	};
    bool endPacketData(void);

    /*template <typename T>
    T readByTypeName(char* aName, unsigned int aOffset)
    {
        ///Thats all the types that can be read... for now. some day these functions will be better.
        if(aName == (char*)typeid(short))
            return readShort(aOffset);
        if(aName == (char*)typeid(unsigned short))
            return readUShort(aOffset);
        if(aName == (char*)typeid(int))
            return readInteger(aOffset);
        if(aName == (char*)typeid(unsigned int))
            return readUInteger(aOffset);
        if(aName == (char*)typeid(char))
            return readChar(aOffset);
        if(aName == (char*)typeid(unsigned char))
            return readUChar(aOffset);
    }*/

    template <typename T>
    void getMembData(unsigned int aSection, unsigned int aMember, T &aData)
    {
        if(aSection < mSectionInfo.size())
        {
            if(aMember < mSectionInfo[aSection].size())
            {
                unsigned int offset = mSectionInfo[aSection][aMember].second;
                if(typeid(aData).name() != mSectionInfo[aSection][aMember].first)
                printf("packet: getMemData(), warning passing incorrect type to be filled.\n");
                readDataIntoType(offset, aData);
            }
        }
    }

    template <typename T>
    void readDataIntoType(unsigned short aOffset, T &aData)
    {
        if(typeid(aData) == typeid(short))
        {
            aData = readShort(aOffset);
            return;
        }
        if(typeid(aData) == typeid(unsigned short))
        {
            aData = readUShort(aOffset);
            return;
        }
        if(typeid(aData) == typeid(int))
        {
            aData = readInteger(aOffset);
            return;
        }
        if(typeid(aData) == typeid(unsigned int))
        {
            aData = readUInteger(aOffset);
            return;
        }
        if(typeid(aData) == typeid(char))
        {
            aData = readChar(aOffset);
            return;
        }
        if(typeid(aData) == typeid(unsigned char))
        {
            aData = readUChar(aOffset);
            return;
        }

        printf("failure on type recongnision\n");
    }

    int readInteger(unsigned int offset);
    unsigned int readUInteger(unsigned int offset); ///these return the data at the offset, with the type used.

    short readShort(unsigned int offset);
    unsigned short readUShort(unsigned int offset);

    char readChar(unsigned int offset);
    unsigned char readUChar(unsigned int offset);

	protected:
	private:

	bool mPacking;
	bool mAnalysed;
	unsigned int mSizeAccumulator;
	unsigned int mCurrentDefID;
	unsigned int mTypeIttorator;

	std::vector<std::vector<std::pair<char*, unsigned int> > > mSectionInfo; ///vector<type(name)> and offset(start of section).

    /// header realated information.
	unsigned int mHeaderSize;   /// = 16;
	unsigned char* mData; // the actual data
	unsigned int mEnd;   // the End offset, how far it is to the end of the current dataset
	unsigned int mAlloc;  // amount of memory currently allocated to the packet
	packetDef* mDefinitions;

};
#endif//PACKET_H



