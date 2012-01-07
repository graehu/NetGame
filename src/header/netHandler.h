#ifndef NETHANDLER_H
#define NETHANDLER_H

#include "packet.h"

namespace net
{

class netHandler
{
public:

	virtual void readData(packet* _packet) = 0;
	virtual void writeData(packet* _packet) = 0;

private:

};
}
#endif//NETHANDLER_H
