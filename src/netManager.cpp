#include "header/netManager.h"
using namespace net;

netManager::netManager(unsigned short _protocolID, float _timeout , unsigned int _maxSequence)
{

	m_network = new network(_protocolID, _timeout, _maxSequence);
	m_entityHandler = new entityHandler;
	m_eventHandler = new eventHandler;
	m_network->registerHandler(m_entityHandler);
	m_network->registerHandler(m_eventHandler);

	m_host = false;
}

bool netManager::init(bool _host, int _port)
{
  m_host = _host;
  if(m_network->init(_host, _port) == 1)
	  return true;
  else
  {
	  if(!_host)
	  {
		  //TODO FINISH DEISGN!
		  // This is kinda where i left off.
		  // I THINK that i'm supposed to try make it send init packets...
		  // Then have this dude subscribe to the e_initedEvent...
		  // This design is terrible (FML)
		  m_eventHandler->causeEvent(e_initEvent);
	  }
  }
}

bool netManager::update(float _deltaTime)
{
	return m_network->update(_deltaTime);
}

netEntity* netManager::getEntity(unsigned int _element)
{
	return m_entityHandler->getEntity(_element);
}
void netManager::draw(void)
{
	m_entityHandler->draw();
}
