#ifndef NETSUBSCRIBER_H
#define NETSUBSCRIBER_H

enum events
{
	e_initEvent = 0,
	e_initedEvent,
	e_addEntityEvent,
	e_addPlayerEvent,
	e_totalEvents
};
class netSubscriber
{
public:
	virtual void notify(events _event) = 0;

};

#endif//NETSUBSCRIBER_H
