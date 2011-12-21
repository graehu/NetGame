#ifndef NET_H
#define NET_H

/// Include

#include "address.h"
#include "socket.h"
#include "connection.h"
#include "reliabilitySystem.h"
#include "packetqueue.h"
#include "flowControl.h"
#include "entity.hpp"
//#include "player.h"
#include "renderer.h"
#include "packetDef.h"

//#include <map>

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
        eUninitialised = 0,
        eInitialised,
        eUpdating
    };

    class network : protected connection
    {
        public:

        network(unsigned short protocolId, float timeout , unsigned int max_sequence = 0xFFFFFFFF);
        ~network();
        bool init(bool aHost, int aPort);
        bool update(float aDeltaTime);
        void draw(void);
        void addEntity(void);
        entity* getEntity(unsigned int aElement);
        protected:
        private:

        void initEntity(unsigned short packetSender, unsigned short accessKey);

        packetDef mDefines;

	bool mHost;
        /// this will have to be something like this eventually
        /// vector<pair<entity*,vector<unsigned short(sendKeys)> > >
        vector<entity*> mEntities;
        struct enInfo
        {
            enInfo(unsigned short aEnKey = 0, entityState aState = eUninitialised) :
            mEnKey(aEnKey),
            mState(aState){};

            unsigned short mEnKey;
            entityState mState;
        };
        ///state list PER connection...
        ///this might want to be a vector<pair<there, >> mEntityConnKeys
        vector<vector<enInfo> >mEnUpdate; ///these are all the keys a client knows about.
        unsigned int mPacketSize; /// this is the maximum size of a packet expect by the network
                                  /// implemented in net.cpp

        Renderer myRender;

		///i probably need to have flow control per connection in the connection class
        ///some sort of input pointer.


    };

}



#endif
