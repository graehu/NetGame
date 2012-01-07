#ifndef CONNECTION_H
#define CONNECTION_H

#include "stats.h"
#include "flowControl.h"
#include "socket.h" /// has address in it.
//#include "packet.h"
#include "readPacket.h"
#include "writePacket.h"

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

    connection(unsigned short _protocolID, float _timeout, unsigned int _maxSequence = 0xFFFFFFFF );
    virtual ~connection();
    bool start(int _port);
    void stop();
    void connect(const address &_address);

    void update(float _deltaTime);
    bool sendPacket(unsigned short _key, float _deltaTime);
    int receivePacket(unsigned int _size);
    int getHeaderSize() const;
    unsigned short getMailListSize(void){return m_mailList.size();}

protected:

    //TODO make these two packets into a singleton, packet interface... thing.
    readPacket m_receivePacket;
    writePacket m_sendPacket;

    unsigned char* m_receiveData;
    unsigned char* m_sendData;

    unsigned int m_port;

private:

    enum state
    {
        e_disconnected = 0,
        e_connecting,
        e_connected
    };

    struct sender
    {
        sender(unsigned int max):
			m_stats(max),
			m_state(e_disconnected),
			m_timeoutAccumulator(0),
			m_sendAccumulator(0){};

        state m_state;
        float m_timeoutAccumulator;
        float m_sendAccumulator;
        address m_address;
        stats m_stats;
        flowControl m_flow;
    };

    unsigned short m_protocolID;                                  /// the protocal id
    float m_timeout;                                              /// how long before someone will time-out
    unsigned int m_maxSequence;                                  /// max sequence for reliablesystem
    bool m_running;                                               /// is this connection running

    socket m_socket;                                              /// this is a socket
    std::vector<std::pair<sender*, unsigned short> > m_mailList;  /// these are all the connections, and their send keys.
    std::vector<unsigned short> m_keyPool;                        /// these are keys that can be reused
    std::vector<unsigned short> m_newConnKeys;                     /// this stores all the new connection's Keys

    float m_sendAccumulator;                                      /// this is so as the rate at which initialisation packets are sent
                                                                /// can be maintained
};

}

#endif//CONNECTION_H
