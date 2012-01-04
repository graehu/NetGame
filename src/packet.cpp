#include "header/packet.h"
#include <cstdio>
#include <cstdarg>

packet::packet()
{
	m_alloc = 0;
	mPacking = false;
	m_headerSize = m_end = 16;
    m_data = new unsigned char[m_headerSize];
	mTypeIttorator = 0;
    mCurrentDefID = 0;
    mSizeAccumulator = 0;
    m_analysed = false;

}

packet::~packet()
{ ///this may require more later.
    if(m_data != 0)
	delete m_data;
}


bool packet::beginPacketData(unsigned int _packetDefID)
{
    if(_packetDefID < m_definitions->size())
    {
        assert(!mPacking);
        if(!mPacking)
        {
            mCurrentDefID = _packetDefID;
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
        if(mSizeAccumulator != m_definitions->getDefSize(mCurrentDefID))
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
    unsigned short offset = m_headerSize;
    unsigned short secType = readUShort(offset);
    bool data = true;
    unsigned int i = 0;
    unsigned int ii = 0;

    if(secType < m_definitions->size())
    {
        while(data)
        {
            while(m_definitions->getMembTypeName(secType, ii) != 0)
            {

                if(i >= mSectionInfo.size())
                {
                    //printf("ok wtf\n");
                    mSectionInfo.push_back(std::vector<std::pair<char*, unsigned int> >());
                }

                if(ii >= mSectionInfo[i].size())
                {
                    mSectionInfo[i].push_back(std::pair<char*,unsigned short>(m_definitions->getMembTypeName(secType, ii), offset));
                    //printf("ok wtf\n");
                }
                else
                {
                    mSectionInfo[i][ii].first = m_definitions->getMembTypeName(secType, ii);
                    mSectionInfo[i][ii].second = offset;

                }

                offset += m_definitions->getTypeSizeByName(m_definitions->getMembTypeName(secType, ii));
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

    m_analysed = true;
}

unsigned int packet::getSectionType(unsigned int _section)
{
    if(_section < mSectionInfo.size() && _section >= 0)
    {
        if(!mSectionInfo[_section].empty())
        return readUShort(mSectionInfo[_section][0].second);

        //printf("packet: getSectionType(), error section = %i, no such section\n", aSection);
        return 0;
    }
    else
    {
        //printf("packet: getSectionType(), error section = %i, no such section\n", aSection);
        return 0;
    }
}

unsigned int packet::getSectionStart(unsigned int _section)
{
    if(_section < mSectionInfo.size())
    {
        return mSectionInfo[_section][0].second;
    }
}

bool packet::setAlloc(unsigned int _alloc)
{

	if(_alloc > m_alloc)
	{
		unsigned char* temp;
		temp = m_data;
		m_data = new unsigned char[_alloc];
		if(temp != 0)
		{
            memcpy(&m_data[0], temp, m_end);
            delete temp;
		}
		m_alloc = _alloc;

		return false;
	}
	return true;
}

bool packet::pushData(const unsigned char _data[], unsigned int _size)
{
	if((m_end + _size) > m_alloc)
	{
		unsigned char* temp;
		temp = m_data;
		m_data = new unsigned char[(m_end + _size)*2];
		if(temp != 0)
		{
            memcpy(&m_data[0], temp, m_end);
            delete temp;
		}
		m_alloc = (m_end + _size)*2;
	}

	memcpy(&m_data[m_end], _data, _size);
	m_end += _size;
	return false;
}

unsigned int packet::readUInteger(unsigned int _offset)
{
    if(_offset + sizeof(short)> m_end)
    {
        printf("Packet: readUInteger(): offset too large.\n");
        return 0;
    }

    unsigned int number = (((unsigned int)m_data[0+_offset] << 24) | ((unsigned int)m_data[1+_offset] << 16) |
                           ((unsigned int)m_data[2+_offset] << 8)  | ((unsigned int)m_data[3+_offset]));
    return number;
}

int packet::readInteger(unsigned int _offset)
{
    if(_offset + sizeof(int) > m_end)
    {
        printf("Packet: readInteger(): offset too large.\n");
        return 0;
    }

    int number = (((int)m_data[0+_offset] << 24) | ((int)m_data[1+_offset] << 16) |
                  ((int)m_data[2+_offset] << 8)  | ((int)m_data[3+_offset]));
    return number;
}

unsigned short packet::readUShort(unsigned int _offset)
{

    if(_offset + sizeof(short) > m_end)
    {
        printf("Packet: readUShort(): offset too large.\n");
        return 0;
    }

    unsigned short number = (((unsigned short)m_data[0+_offset] << 8) |
                    ((unsigned short)m_data[1+_offset]));

    return number;
}

short packet::readShort(unsigned int _offset)
{
    if(_offset+sizeof(short) > m_end)
    {
        printf("Packet: readShort(): offset too large.\n");
        return 0;
    }
    short number = (((short)m_data[0+_offset] << 8) |
                    ((short)m_data[1+_offset]));

    return number;
}


unsigned char packet::readUChar(unsigned int _offset)
{

    if(_offset + sizeof(unsigned char) > m_end)
    {
        printf("Packet: readUChar(): offset too large.\n");
        return 0;
    }

    unsigned char character = ((unsigned char)m_data[_offset]);

    return character;
}

char packet::readChar(unsigned int _offset)
{
    if(_offset+sizeof(short) > m_end)
    {
        printf("Packet: readChar(): offset too large.\n");
        return 0;
    }
    char character = ((char)m_data[_offset]);

    return character;
}
