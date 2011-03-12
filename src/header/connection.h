#ifndef CONNECTION_H
#define CONNECTION_H

#include "reliabilitySystem.h"
#include "socket.h" //has address in it.
#include "packet.h"

//#include <list>

///friendly user-data-group encapsulator

///the init packet
///{
///short protocolId;    // this is just a security measure
///short sendKey;       // this is the key you send, so as they can directly access you in their mail List
///short recieveKey;    // this is the key to access them in your mailList
///}

namespace net
{

class connection
{
public:



    connection(unsigned short aProtocolId, float aTimeout, unsigned int aMax_sequence = 0xFFFFFFFF );
    virtual ~connection();
    bool start(int port);
    void stop();
    void connect(const address & address);

    bool isRunning() const;                 ///GOING TO DELETE YOU HAHAHA
    void listen();                          ///GOING TO DELETE YOU HAHAHA
    bool isConnecting() const;              ///GOING TO DELETE YOU HAHAHA
    bool connectFailed() const;             ///GOING TO DELETE YOU HAHAHA
    bool isConnected() {return connected;}  ///GOING TO DELETE YOU HAHAHA
    bool isListening() const;               ///GOING TO DELETE YOU HAHAHA

    void update(float deltaTime);
    bool sendPacket(const unsigned char data[], unsigned int size, unsigned short key = 0);
    int receivePacket(unsigned char data[], int size);
    int getHeaderSize() const;

    ReliabilitySystem & getReliabilitySystem()
    {
            if(mMailList.size() > 0)
            {

                return mMailList.front().first->mStatistics;
            }
    }

protected:

    void onStart() {}
    void onStop() {}
    void onConnect() {}
    void onDisconnect() {}

    /// Utitlity funcitions

    void writeInit(unsigned char * init, unsigned short sendKey, unsigned short recieveKey);
    void readInit(const unsigned char * init, unsigned short & sendKey, unsigned short & recieveKey);
    void writeHeader(unsigned char * header, unsigned short sendKey, unsigned int sequence, unsigned int ack, unsigned int ack_bits);
    void readHeader(const unsigned char * header,unsigned short & sendKey, unsigned int & sequence, unsigned int & ack, unsigned int & ack_bits);


private:



    enum state
    {
        eDisconnected = 0,
        eConnectFail,
        eConnecting,
        eConnected
    };


    struct sender                                            ///this happened after the great merge of reliability and connection.
    {
        sender(unsigned int max):
			mStatistics(max),
			mState(eDisconnected),
			mTimeoutAccumulator(0){};


        state mState;
        float mTimeoutAccumulator;
        address mAddress;
        ReliabilitySystem mStatistics;
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
	packet mPacket;
};

}

#endif//CONNECTION_H
