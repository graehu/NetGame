#ifndef PACKET_H
#define PACKET_H

#include "packetDef.h"
#include <cstring>
#include <cstdio>
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
	bool pushData(const unsigned char _data[], unsigned int _size);
	bool clearPacket(void){m_end = m_headerSize; m_analysed = false; return false;}
	bool setAlloc(unsigned int aAlloc);
	void setEnd(unsigned int _end){if(_end >! m_alloc && _end > m_headerSize)m_end = _end;}
	unsigned int getAlloc(void) {return m_alloc;}
	unsigned int getEnd(void){return m_end;}
	unsigned char* getData(void){return m_data;}
	unsigned short getType(void);

    /// header realated functionality.
	unsigned int getHeaderSize(void){return m_headerSize;}
	unsigned short readProtocolId(void){return readUShort(0);}
    unsigned short readKey(void){return readUShort(2);}
    unsigned int readSequence(void){return readUInteger(4);}
    unsigned int readAck(void){return readUInteger(8);}
    unsigned int readAckBits(void){return readUInteger(12);}

    void writeProtocolId(unsigned short _ID){writeData(_ID,0);}
    void writeKey(unsigned short _key){writeData(_key,2);}
    void writeSequence(unsigned int _seq){writeData(_seq,4);}
    void writeAck(unsigned int _ack){writeData(_ack,8);}
    void writeAckBits(unsigned int _ackBits){writeData(_ackBits,12);}
    /// header realated functionality.

    void setDefinitions(packetDef* _defines){m_definitions = _defines;} ///this est bad.
    packetDef* getDefinitions(void){return m_definitions;}

    void analysePacket(void);
    unsigned int getSectionType(unsigned int _section);
    unsigned int getSectionStart(unsigned int _section);

    /// attempt to make a generic data reader, failed.
    /*template <typename T>
    void readData(unsigned int aOffset, T &_data)
    {
        //unsigned char temp[sizeof(_data)] =  {0};
        T temp = 0;
        for(int i = 0; i < sizeof(_data)-1; i++)
            temp = ((mData[aOffset+i] << ((sizeof(_data)-1)*8 -(i*8))) | (mData[aOffset+(sizeof(_data)-1)]));
            ((t)mData[aOffset])

            printf("hmmm: %i\n", temp);

        //_data = (T)temp;
    }*/

	/// stupid template functions can't be declared in one file and defined in another.
	template <typename T>
	bool pushData(T _data)
	{
	    if(!mPacking){
		unsigned int size = sizeof(_data);
		if((m_end + size) > m_alloc)
		{
			unsigned char* temp;
			temp = m_data;
			m_data = new unsigned char[(m_end + size)*2];
            if(temp != 0)
            {
                memcpy(&m_data[0], temp, m_end);
                delete temp;
            }
			m_alloc = (m_end + size)*2;
		}

		writeData(_data,m_end);

		m_end += size;
		return false;}else return true;
	};

    ///bad generic data writer.
	template <typename T>
    bool writeData(T _data, unsigned int _offset)
	{
		unsigned int size = sizeof(_data);

		if((_offset + size) > m_alloc)
		{
		    printf("Packet: writeData() error, offset too large.\n");
		    return true;
		}

		for(unsigned int i = 0; i < size; i++)
		{
			if (i > 0)
				m_data[_offset+i] = (_data >> (size-(i+1))*8);
			else
				m_data[_offset+i] = ((_data >> (size-(i+1))*8) & 0xFF);
		}
		return false;
	}

    bool beginPacketData(unsigned int _packetDefID);
  	template <typename T>
	bool safePushData(T _data)
	{
	    if(mPacking)
	    {
            if(typeid(T).name() == m_definitions->getMembTypeName(mCurrentDefID, mTypeIttorator))
            {
                unsigned int size = sizeof(_data);
                if((m_end + size) > m_alloc)
                {
                    unsigned char* temp;
                    temp = m_data;
                    m_data = new unsigned char[(m_end + size)*2];
                    if(temp != 0)
                    {
                        memcpy(&m_data[0], temp, m_end);
                        delete temp;
                    }
                    m_alloc = (m_end + size)*2;
                }
                //printf("i'm pushing, right?\n");
                writeData(_data,m_end);

                mSizeAccumulator += size;
                m_end += size;
                mTypeIttorator++;
                return false;
            }
            else
            {
                printf("packet: safePushData(), error expected:%s passed:%s\n",
                       (const char*)m_definitions->getMembTypeName(mCurrentDefID, mTypeIttorator),
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
    void getMembData(unsigned int aSection, unsigned int aMember, T &_data)
    {
        if(aSection < mSectionInfo.size())
        {
            if(aMember < mSectionInfo[aSection].size())
            {
                unsigned int offset = mSectionInfo[aSection][aMember].second;
                if(typeid(_data).name() != mSectionInfo[aSection][aMember].first)
                printf("packet: getMemData(), warning passing incorrect type to be filled.\n");
                readDataIntoType(offset, _data);
            }
        }
    }

    template <typename T>
    void readDataIntoType(unsigned short aOffset, T &_data)
    {
        if(typeid(_data) == typeid(short))
        {
            _data = readShort(aOffset);
            return;
        }
        if(typeid(_data) == typeid(unsigned short))
        {
            _data = readUShort(aOffset);
            return;
        }
        if(typeid(_data) == typeid(int))
        {
            _data = readInteger(aOffset);
            return;
        }
        if(typeid(_data) == typeid(unsigned int))
        {
            _data = readUInteger(aOffset);
            return;
        }
        if(typeid(_data) == typeid(char))
        {
            _data = readChar(aOffset);
            return;
        }
        if(typeid(_data) == typeid(unsigned char))
        {
            _data = readUChar(aOffset);
            return;
        }

        printf("failure on type recognition\n");
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
	bool m_analysed;
	unsigned int mSizeAccumulator;
	unsigned int mCurrentDefID;
	unsigned int mTypeIttorator;

	std::vector<std::vector<std::pair<char*, unsigned int> > > mSectionInfo; ///vector<type(name)> and offset(start of section).

    /// header related information.
	unsigned int m_headerSize;   /// = 16;
	unsigned char* m_data; // the actual data
	unsigned int m_end;   // the End offset, how far it is to the end of the current data-set
	unsigned int m_alloc;  // amount of memory currently allocated to the packet
	packetDef* m_definitions;

};
#endif//PACKET_H



