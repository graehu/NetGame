#ifndef ENTITYHANDLER_H
#define ENTITYHANDLER_H
#include "netHandler.h"
#include "netEntity.h"
#include "subscriber.h"
#include "renderer.h"
#include <vector>

namespace net
{
enum entityState
{
    e_uninitialised = 0,
    e_initialised,
    e_updating
};

class entityHandler : public netHandler, public netSubscriber
{
public:

	void writeData(packet* _packet);
	void readData(packet* _packet);


	void notify(events _event);
	void draw(void);

	netEntity* getEntity(unsigned int _element);

private:

	void initEntity(unsigned short _packetSender, unsigned short _accessKey);

	struct enInfo
		{
			enInfo(unsigned short _enKey = 0, entityState _state = e_uninitialised) :
			m_enKey(_enKey),
			m_state(_state){};

			unsigned short m_enKey;
			entityState m_state;
		};
    Renderer m_renderer;
	std::vector<std::vector<enInfo> >m_enUpdate; ///these are all the keys a client knows about.


	std::vector<netEntity*> m_entities;
};

}

#endif//ENTITYHANDLER_H
