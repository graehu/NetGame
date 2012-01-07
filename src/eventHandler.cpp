#include "header/eventHandler.h"
using namespace net;

eventHandler::eventHandler()
{

	m_definitions.push_back(new packetDef);
	m_definitions[e_initEvent]->beginDefinition();
	{
		m_definitions[e_initEvent]->pushType((unsigned short)0); //event type: e_initialiseEvent
	}
	m_definitions[e_initEvent]->endDefinition();

	m_definitions.push_back(new packetDef);
	m_definitions[e_initedEvent]->beginDefinition();
	{
		m_definitions[e_initedEvent]->pushType((unsigned short)0); //event type: e_initialiseEvent
	}
	m_definitions[e_initedEvent]->endDefinition();

	m_definitions.push_back(new packetDef);
	m_definitions[e_addEntityEvent]->beginDefinition();
	{
		m_definitions[e_addEntityEvent]->pushType((unsigned short)0); //event type: e_addEntityEvent
		m_definitions[e_addEntityEvent]->pushType((unsigned short)0); //The entity ID.
	}
	m_definitions[e_addEntityEvent]->endDefinition();

	//The host will be the only one to send these kinda packets.
	//He's basically telling you what your player ID is. (what entity belongs to you.)
	m_definitions.push_back(new packetDef);
	m_definitions[e_addPlayerEvent]->beginDefinition();
	{
		m_definitions[e_addPlayerEvent]->pushType((unsigned short)0); //event type: e_addPlayerEvent
		m_definitions[e_addPlayerEvent]->pushType((unsigned short)0); //The entity ID.
	}
	m_definitions[e_addPlayerEvent]->endDefinition();
	entityCount = 0;
}

void eventHandler::writeData(packet* _packet)
{
	unsigned char* p_data = (_packet->getData()+_packet->getHeaderSize());
	unsigned short offset = 0;
	for(unsigned int i = 0; i < m_events.size(); i++)
	{
		switch(m_events[i])
		{
		case e_addEntityEvent:
			break;
		case e_addPlayerEvent:
			break;
		}
	}
}
void eventHandler::readData(packet* _packet)
{

}

void eventHandler::fireEvent(events _event)
{

}
