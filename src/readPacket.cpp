#include "header/readPacket.h"
#include "header/dataUtils.h"
using namespace net;

unsigned short readPacket::readProtocolId(void)
{
	return dataUtils::instance().readUShort(&m_data[0]);
}
unsigned short readPacket::readKey(void)
{
	return dataUtils::instance().readUShort(&m_data[2]);
}
unsigned int readPacket::readSequence(void)
{
	return dataUtils::instance().readUInteger(&m_data[4]);
}
unsigned int readPacket::readAck(void)
{
	return dataUtils::instance().readUInteger(&m_data[8]);
}
unsigned int readPacket::readAckBits(void)
{
	return dataUtils::instance().readUInteger(&m_data[12]);
}
unsigned short readPacket::readHandleID(void)
{
	return dataUtils::instance().readUShort(&m_data[16]);
}

