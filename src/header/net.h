#ifndef NET_H
#define NET_H

//#include <vector>
//#include <map>
//#include <stack>
//#include <list>
//#include <algorithm>
//#include <functional>

//Include

#include "address.h"
#include "socket.h"
#include "connection.h"
#include "reliabilitySystem.h"
#include "packetqueue.h"
#include "flowControl.h"
#include "entity.hpp"
#include "player.h"

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

    enum netType
    {
        net_server,
        net_client
    };

    class network : connection
    {
        public:

            network(unsigned short protocolId, float timeout , unsigned int max_sequence = 0xFFFFFFFF);
            ~network();
            bool init(netType, int port);
			bool update(void);

        protected:

        private:

        netType type;
        std::vector<entity*> entities; //entities the net app is aware of
        FlowControl flowControl;

		///i probably need to have flow control per connection in the connection class
        //some sort of input pointer.
    };

}


#endif
