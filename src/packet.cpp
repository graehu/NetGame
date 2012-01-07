#include "header/packet.h"

#include <cstdio>
#include <cstdarg>
using namespace net;

packet::packet()
{
	m_alloc = 0;
	m_packing = false;
	m_headerSize = m_end = 16;
    m_data = new unsigned char[m_headerSize];
    m_sizeAccumulator = 0;
	//m_typeIttorator = 0;
    //m_currentDefID = 0;
}

packet::~packet()
{ ///this may require more later.
    if(m_data != 0)
	delete m_data;
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









///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
///Code Graveyard













/*
bool packet::beginPacketData(unsigned int _packetDefID)
{
	unsigned int numOfDefs = defRegister::instance().getSize()-1;
    if(_packetDefID < numOfDefs)
    {
        assert(!m_packing);
        if(!m_packing)
        {
            m_currentDefID = _packetDefID;
            m_packing = true;
            safePushData((unsigned short)m_currentDefID);
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
    if(m_packing)
    {
    	unsigned int size = defRegister::instance().getDef(m_currentDefID).getSize();
        if(m_sizeAccumulator != size)
        {
            printf("packet: endPacketData(), warning packet size inadequate.\n");
            //assert(false);
        }
        m_sizeAccumulator = 0;
        m_typeIttorator = 0;
        m_currentDefID = 0;
        m_packing = false;
        return false;
    }
    else return true;
}


void packet::analysePacket(void)
{
    unsigned short offset = m_headerSize;
    unsigned short secType = readUShort(offset);
    unsigned int size = defRegister::instance().getSize()-1;
    bool data = true;
    unsigned int i = 0;
    unsigned int ii = 0;

    m_sectionIDs.push_back(secType);

    if(secType < size)
    {
        while(data)
        {
        	char* name = defRegister::instance().getDef(secType).getMembTypeName(ii);

            while(name != 0)
            {
            	packType type;
                if(i >= m_sectionInfo.size())
                    m_sectionInfo.push_back(std::vector<std::pair<char*, unsigned int> >());

                if(ii >= m_sectionInfo[i].size())
                	m_sectionInfo[i].push_back(std::pair<char*,unsigned short>(name,offset));
                else
                {
                    m_sectionInfo[i][ii].first = name;
                    m_sectionInfo[i][ii].second = offset;

                }
                offset += defRegister::instance().getTypeSizeByName(name);
                ii++;
                name = defRegister::instance().getDef(secType).getMembTypeName(ii);
            }
            secType = readUShort(offset);
            m_sectionIDs.push_back(secType);
            ii = 0;
            i++;
            if(secType == 0)
                data = false;
        }
    }
    else
    {
    	//printf("last sectype: %i\n");
    	assert(false);
    	return;
    }
}

unsigned int packet::getSectionType(unsigned int _section)
{
    if(_section < m_sectionInfo.size() && _section >= 0)
    {
        if(!m_sectionInfo[_section].empty())
        return readUShort(m_sectionInfo[_section][0].second);

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
    if(_section < m_sectionInfo.size())
    {
        return m_sectionInfo[_section][0].second;
    }
    return 0;
}*/
