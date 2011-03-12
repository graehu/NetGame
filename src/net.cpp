#include "header/net.h"

using namespace net;

network::network(unsigned short protocolId, float timeout , unsigned int max_sequence) :
    connection(protocolId, timeout, max_sequence)
{

    type = net_client;
    entities.clear(); //entities the net app is aware of
    flowControl.Reset();
}

network::~network()
{

}


bool network::init(netType type, int port)
{
	if (!start(port))
	{
		printf("could not start connection on port %d\n", port);
		return 1;
	}

	if(type == net_client)
	{
	    entity* me = new player;
	    entities.push_back(me);

	}
    return false;
}

bool network::update(void)
{

	//receivePacket();


	return false;

}
