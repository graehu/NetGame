#ifndef CONNECTION_H
#define CONNECTION_H

#include "reliabilitySystem.h"
#include "flowControl.h"
#include "socket.h" /// has address in it.
#include "packet.h"

///friendly user-data-group encapsulator

///the init packet
///{
///short protocolId;    // this is just a security measure
///short sendKey;       // this is the key you send, so as they can directly access you in their mail List
///short ReceiveKey;    // this is the key to access them in your mailList
///}

namespace net
{

class connection
{
public:



    connection(unsigned short aProtocolId, float aTimeout, unsigned int aMax_sequence = 0xFFFFFFFF );
    virtual ~connection();
    bool start(int aPort);
    void stop();
    void connect(const address & address);

    bool isConnected(void) {return connected;}

    void update(float deltaTime);
    bool sendPacket(unsigned short aKey, float aDeltaTime);
    int receivePacket(unsigned int aSize);
    int getHeaderSize() const;

    ReliabilitySystem & getReliabilitySystem()
    {
            if(mMailList.size() > 0)
            {

                return mMailList.front().first->mStatistics;
            }
    }

protected:

    packet mReceivePacket;
	packet mSendPacket;
    unsigned int mPort;
private:

    enum state
    {
        eDisconnected = 0,
        eConnectFail,
        eConnecting,
        eConnected
    };

    struct sender
    {
        sender(unsigned int max):
			mStatistics(max),
			mState(eDisconnected),
			mTimeoutAccumulator(0),
			mSendAccumulator(0){};

        state mState;
        float mTimeoutAccumulator;
        float mSendAccumulator;
        address mAddress;
        ReliabilitySystem mStatistics;
        FlowControl mFlow;
    };

    unsigned short mProtocolId;                                  /// the protocal id
    float mTimeout;                                              /// how long before someone will time-out
    unsigned int mMax_sequence;                                  /// max sequence for reliablesystem
    bool mRunning;                                               /// is this connection running
    bool connected; ///legacy.

    Socket mSocket;                                              /// this is a socket
    std::vector<std::pair<sender*, unsigned short> > mMailList;  /// these are all the connections, and their send keys.
    std::vector<unsigned short> mKeyPool;                        /// these are keys that can be reused
    std::vector<unsigned short> mNewConnKeys;                     /// this stores all the new connection's Keys

    float mSendAccumulator;                                      /// this is so as the rate at which initialisation packets are sent
                                                                /// can be maintained
};

}

#endif//CONNECTION_H
