#include "header/netEntity.h"
#include "header/dataUtils.h"

using namespace net;

netEntity::netEntity()
{
    m_type = e_netEntity;
    m_command = ' ';

	static bool s_defInit = false;
    if(!s_defInit)
    {
    	////////////////////////////////////////////////////////
    	//update packet.
    	m_define.beginDefinition();
    		for(unsigned int i = 0; i < 2; i++)
    			m_define.pushType((float)0);	 // m_x, m_y
    	m_define.pushType((unsigned char)0); // m_commands
    	m_define.endDefinition();
    	//
    	////////////////////////////////////////////////////////
		printf("NetEntity: I should only see this once per console\n");
		s_defInit =  true;
    }
}

void netEntity::readPacket(unsigned char* _data)
{
	m_x = (float)dataUtils::instance().readUInteger(&_data[0]);
	m_y = (float)dataUtils::instance().readUInteger(&_data[4]);
	m_command = dataUtils::instance().readUChar(&_data[8]);
}

void netEntity::writePacket(unsigned char* _data)
{
	dataUtils::instance().writeData((unsigned int)m_x, &_data[0]);
	dataUtils::instance().writeData((unsigned int)m_x, &_data[sizeof(unsigned int)]);
	dataUtils::instance().writeData(m_command, &_data[sizeof(unsigned int)*2]);
}

void netEntity::move(void)
{
    if((m_command&1) > 0) m_y-=1;
    if((m_command&2) > 0) m_x-=1;
    if((m_command&4) > 0) m_y+=1;
    if((m_command&8) > 0) m_x+=1;
}

