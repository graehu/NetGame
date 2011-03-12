#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "header/net.h"

//#define SHOW_ACKS

using namespace std;
using namespace net;

const int ServerPort = 8000;
const int ClientPort = 8001;
const unsigned short ProtocolId = 80;//0xf00d;
const float DeltaTime = 1.0f / 30.0f;
const float SendRate = 1.0f / 30.0f;
const float TimeOut = 10.0f;
const int PacketSize = 256;


// ----------------------------------------------

int main( int argc, char * argv[] )
{
	// parse command line
  //random comment

	enum Mode
	{
		Client,
		Server
	};

	Mode mode = Server;
	address myAddress;

	if (!(argc >= 2))
	{
		//int a,b,c,d;
		//if ( sscanf( argv[1], "%d.%d.%d.%d", &a, &b, &c, &d ) )
		//{
			mode = Client;
			myAddress = address(127,0,0,1,ServerPort);
		//}
	}

	// initialize
	int error = InitializeSockets();
	if ( error != 0 )
	{
		printf( "failed to initialize sockets, error(%i)\n",error);
		return 1;
	}


	connection connection( ProtocolId, TimeOut );

	const int port = mode == Server ? ServerPort : ClientPort;

	if (!connection.start(port))
	{
		printf( "could not start connection on port %d\n", port );
		return 1;
	}

	if (mode == Client)
		connection.connect(myAddress);


	bool connected = false;
	float sendAccumulator = 0.0f;
	float statsAccumulator = 0.0f;

	FlowControl flowControl;

	while ( true )
	{
		// update flow control

		if ( connection.isConnected() )
			flowControl.Update( DeltaTime, connection.getReliabilitySystem().GetRoundTripTime() * 1000.0f );

		const float sendRate = flowControl.GetSendRate();




		// detect changes in connection state

		if ( mode == Server && connected && !connection.isConnected() )
		{
			flowControl.Reset();
			printf( "reset flow control\n" );
			connected = false;
		}

		if ( !connected && connection.isConnected() )
		{
			printf( "client connected to server\n" );
			connected = true;
		}

		/*if ( !connected && connection.ConnectFailed() )
		{
			printf( "connection failed\n" );
			break;
		}*/

		// send and receive packets

		sendAccumulator += DeltaTime;

		while (sendAccumulator > 1.0f / sendRate)
		{
			unsigned char packet[PacketSize];
			memset(packet, 0, sizeof(packet));
			connection.sendPacket(packet, sizeof(packet));
			sendAccumulator -= 1.0f / sendRate;
		}

		while (true)
		{
			unsigned char packet[256];
			int bytes_read = connection.receivePacket(packet, sizeof(packet));
			if (bytes_read == 0)
				break;
		}

		// show packets that were acked this frame

		#ifdef SHOW_ACKS
		if(connected)
		{
		unsigned int * acks = NULL;
		int ack_count = 0;
		connection.GetReliabilitySystem().GetAcks( &acks, ack_count );
		if ( ack_count > 0 )
		{
			printf( "acks: %d", acks[0] );
			for ( int i = 1; i < ack_count; ++i )
				printf( ",%d", acks[i] );
			printf( "\n" );
		}
		}
		#endif

		// update connection

		connection.update( DeltaTime );

		// show connection stats

		statsAccumulator += DeltaTime;

		while ( statsAccumulator >= 0.25f && connection.isConnected() )
		{

			float rtt = connection.getReliabilitySystem().GetRoundTripTime();

			unsigned int sent_packets = connection.getReliabilitySystem().GetSentPackets();
			unsigned int acked_packets = connection.getReliabilitySystem().GetAckedPackets();
			unsigned int lost_packets = connection.getReliabilitySystem().GetLostPackets();

			float sent_bandwidth = connection.getReliabilitySystem().GetSentBandwidth();
			float acked_bandwidth = connection.getReliabilitySystem().GetAckedBandwidth();

			printf( "rtt %.1fms, sent %d, acked %d, lost %d (%.1f%%), sent bandwidth = %.1fkbps, acked bandwidth = %.1fkbps\n",
				rtt * 1000.0f, sent_packets, acked_packets, lost_packets,
				sent_packets > 0.0f ? (float) lost_packets / (float) sent_packets * 100.0f : 0.0f,
				sent_bandwidth, acked_bandwidth );

			statsAccumulator -= 0.25f;
		}

		waitsecs( DeltaTime );
	}

	getchar();

	ShutdownSockets();

	return 0;
}
