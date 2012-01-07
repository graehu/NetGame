#ifndef READPACKET_H
#define READPACKET_H

#include "packet.h"

namespace net
{
class readPacket : public packet
{
public:

	unsigned short readProtocolId(void);
    unsigned short readKey(void); // Who's sending to me.
    unsigned int readSequence(void);
    unsigned int readAck(void);
    unsigned int readAckBits(void);
    unsigned short readHandleID(void);
};
}
#endif//READPACKET_H
