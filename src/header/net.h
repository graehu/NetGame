#ifndef NET_H
#define NET_H

/// Include

#include "address.h"
#include "socket.h"
#include "connection.h"
#include "stats.h"
#include "packetqueue.h"
#include "flowControl.h"
#include "netEntity.h"
#include "renderer.h"
#include "packetDef.h"


using namespace std;

#if PLATFORM == PLATFORM_WINDOWS

inline void waitsecs(float seconds)
{
    Sleep((int)(seconds * 1000.0f));
}

#else

inline void waitsecs(float seconds)
{
    usleep((int)(seconds * 1000000.0f));
}

#endif

namespace net
{

    enum entityState
    {
        e_uninitialised = 0,
        e_initialised,
        e_updating
    };

    class network : protected connection
    {
        public:

        network(unsigned short _protocolID, float _timeout , unsigned int _maxSequence = 0xFFFFFFFF);
        ~network();
        bool init(bool _host, int _port);
        bool update(float _deltaTime);
        void draw(void);
        void addEntity(void);
        netEntity* getEntity(unsigned int _element);
	bool getType(void){return m_host;}
        protected:
        private:

        void initEntity(unsigned short _packetSender, unsigned short _accessKey);

        packetDef m_defines;

	bool m_host;
        /// this will have to be something like this eventually
        /// vector<pair<entity*,vector<unsigned short(sendKeys)> > >
        vector<netEntity*> m_entities;
        struct enInfo
        {
            enInfo(unsigned short _enKey = 0, entityState _state = e_uninitialised) :
            m_enKey(_enKey),
            m_state(_state){};

            unsigned short m_enKey;
            entityState m_state;
        };
        ///state list PER connection...
        ///this might want to be a vector<pair<there, >> mEntityConnKeys
        vector<vector<enInfo> >m_enUpdate; ///these are all the keys a client knows about.
        unsigned int m_packetSize; /// this is the maximum size of a packet expect by the network
                                  /// implemented in net.cpp

        Renderer m_renderer;

    };

}



#endif
