#include "header/netEntity.h"
#include "header/dataUtils.h"

using namespace net;

netEntity::netEntity()
{
    m_type = e_netEntity;
    m_command = ' ';
}

void netEntity::readPacket(packet* _packet)
{
	m_x = (float)_packet->iterRead<int>();
	m_y = (float)_packet->iterRead<int>();
}

void netEntity::writePacket(packet* _packet)
{
	_packet->iterWrite((unsigned int)m_x);
	_packet->iterWrite((unsigned int)m_y);
}

void netEntity::move(void)
{
    if((m_command&1) > 0) m_y-=1;
    if((m_command&2) > 0) m_x-=1;
    if((m_command&4) > 0) m_y+=1;
    if((m_command&8) > 0) m_x+=1;
}

