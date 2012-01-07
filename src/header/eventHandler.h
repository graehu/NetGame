#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H
#include "packetDef.h"
#include "netHandler.h"
#include "subscriber.h"
#include "renderer.h"
#include <vector>


namespace net
{


class eventHandler : public netHandler
{
public:
	eventHandler();
	virtual ~eventHandler(){}

	void writeData(packet* _packet);
	void readData(packet* _packet);

	void subscribe(netSubscriber* _subscriber ,events _event);
	void causeEvent(events _event);


private:

	void fireEvent(events _event);

	std::vector<netSubscriber*> subscriptions[e_totalEvents];
	std::vector<events> m_events;
	std::vector<packetDef*> m_definitions;
	unsigned short entityCount;

};

}

#endif//EVENTHANDLER_H
