#ifndef NETMANAGER_H
#define NETMANAGER_H

#include "net.h"
#include "eventHandler.h"
#include "entityHandler.h"


namespace net
{
class netManager : public netSubscriber
{
public:

	netManager(unsigned short _protocolID, float _timeout , unsigned int _maxSequence = 0xFFFFFFFF);
	bool init(bool _host, int _port);
	bool getType(void){return m_host;}
    bool update(float _deltaTime);
	netEntity* getEntity(unsigned int _element);
	void draw(void);
	void notify(events _event);

private:

	bool m_host;
	network* m_network;
	eventHandler* m_eventHandler;
	entityHandler* m_entityHandler;
};
}

#endif//NETMANAGER_H
