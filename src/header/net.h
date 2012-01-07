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
#include "netHandler.h"


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

        packet* getReceivePacket(void){return &m_receivePacket;}
        packet* getSendPacket(void){return &m_sendPacket;}
        void registerHandler(netHandler* _handler);

        protected:
        private:

        vector<netHandler*> m_handlers;
        //void initEntity(unsigned short _packetSender, unsigned short _accessKey);

        bool m_host;
        unsigned int m_packetSize; /// this is the maximum size of a packet expect by the network



    };

}



#endif
