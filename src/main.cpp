#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <SDL/SDL.h>

#include "header/net.h"

//#define SHOW_ACKS

using namespace std;
using namespace net;

const int ServerPort = 8000;
const int ClientPort = 8001;
const unsigned short ProtocolId = 0xf00d;//80;
const float DeltaTime = 1.0f/60.0f;
const float SendRate = 1.0f / 30.0f;
const float TimeOut = 10.0f;
const int PacketSize = 256;

#define CLIENTS 5

bool input(bool* keys);

enum Keys
{
    eUp,
    eDown,
    eLeft,
    eRight,
    eKeysSize
};

bool input(bool* keys)
{
    for(int i = 0; i < eKeysSize; i++)
        keys[i] = false;

    char* internalKeys = (char*)SDL_GetKeyState(NULL);
    if (internalKeys['w'])
    {
        keys[eUp] = true;
    }
        if (internalKeys['a'])
    {
        keys[eLeft] = true;
    }
        if (internalKeys['s'])
    {
        keys[eDown] = true;
    }
        if (internalKeys['d'])
    {
        keys[eRight] = true;
    }
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        //printf("do i at least get here..\n");
        switch (event.type)
        {
            /*case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case 'w':
                    {
                        keys[eUp] = true;
                        //printf("confused?\n");
                        break;
                    }
                    case 'a':
                    {
                        keys[eLeft] = true;
                        break;
                    }
                    case 's':
                    {
                        keys[eDown] = true;
                        break;
                    }
                    case 'd':
                    {
                        keys[eRight] = true;
                        break;
                    }
                }
                default:
                {
                    break;
                }
            }*/
            case SDL_QUIT:
            {
                return true;
            }
        }
    }
    return false;
}

// ----------------------------------------------


int main(int argc, char * argv[])
{
    /*network myNetwork(0xF00D, TimeOut);

    bool keys[4] = {false,false,false,false};
    while(true)
    {
        if(input(keys) == true) return 0;
        //input(keys);
        if(keys[(int)eUp] == true)
        {
            printf("pressing up\n");
        }
        if(keys[(int)eLeft] == true)
        {
            printf("pressing left\n");
        }
        if(keys[(int)eDown] == true)
        {
            printf("pressing down\n");
        }
        if(keys[(int)eRight] == true)
        {
            printf("pressing right\n");
        }
        myNetwork.draw();
    }
    return 0;

}//*/
/*


    bool keys[4] = {false,false,false,false};
    network server(0xF00D, TimeOut);
    network clients[CLIENTS] = network(0xF00D, TimeOut);

    server.init(eServer, 8000);
    for(int i = 0; i < CLIENTS; i++)
        clients[i].init(eClient, (8001+i));

    while(true)
    {
        server.update(DeltaTime);
        for(int i = 0; i < CLIENTS; i++)
            clients[i].update(DeltaTime);

            /*if(input(keys) == true) return 0;
            if(keys[(int)eUp] == true)
            {
                printf("up\n");
                clients[1].getEntity(0)->setYPos(-10);//myNet.getEntity(0)->getYPos()-1);
            }
            if(keys[(int)eLeft] == true)
            {
                printf("left\n");
                clients[1].getEntity(0)->setXPos(-10);//myNet.getEntity(0)->getXPos()+1);

            }
            if(keys[(int)eDown] == true)
            {
                printf("down\n");
                clients[1].getEntity(0)->setYPos(10);//myNet.getEntity(0)->getYPos()-1);

            }
            if(keys[(int)eRight] == true)
            {
                printf("right\n");
                clients[1].getEntity(0)->setXPos(10);//myNet.getEntity(0)->getXPos()+1);
            }*/
            /*




        waitsecs(DeltaTime);
        server.draw();
        }

    return 0;
    }
    //*/

    network myNet(0xF00D, TimeOut);
    bool keys[4];

    if(!(argc >= 2))
    {
        if(myNet.init(eServer, 8000) == 1)
        {
            bool unbound = true;
            int i = 1;
            while(unbound)
            {
                if(myNet.init(eClient, 8000+i) == 0)
                {
                    unbound = false;
                }
                i++;
            }
        }
    }
    else
    {
        int port = 0;
        if(sscanf(argv[1],"%i",&port))
            myNet.init(eClient, port);
        else
            return 0;
    }

    while(true)
    {
        myNet.update(DeltaTime);
        waitsecs(DeltaTime);
        if(myNet.getType() == eClient)
        {
            if(input(keys) == true) return 0;
            if(keys[(int)eUp] == true)
            {
                printf("up\n");
                //myNet.getEntity(0)->setYPos(-10);//myNet.getEntity(0)->getYPos()-1);
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|1);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-1));}
            if(keys[(int)eLeft] == true)
            {
                printf("left\n");
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|2);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-2));}
            if(keys[(int)eDown] == true)
            {
                printf("down\n");
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|4);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-4));}
            if(keys[(int)eRight] == true)
            {
                printf("right\n");
                myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()|8);
            }
            else{myNet.getEntity(0)->setCommands(myNet.getEntity(0)->getCommands()&(255-8));}
        }
        myNet.draw();
    } return 0;}//*/
    	// parse command line
  //random comment

/*	enum Mode
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

	while (true)
	{


        connection.sendPacket(0,DeltaTime);


		while (true)
		{
			int bytes_read = connection.receivePacket(16);
			if (bytes_read == 0)why are you
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

		connection.update(DeltaTime);

		// show connection stats

		statsAccumulator += DeltaTime;why are you

		while (statsAccumulator >= 0.25f && connection.isConnected())
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

		waitsecs(DeltaTime);
	}

	getchar();

	ShutdownSockets();

	return 0;

//*/
/*
	// parse command line
  //random comment
    address myAddress(127,0,0,1,8000);

	// initialize
	int error = InitializeSockets();
	if ( error != 0 )
	{
		printf( "failed to initialize sockets, error(%i)\n",error);
		return 1;
	}
    bool packets = true;

	connection server(ProtocolId, TimeOut);
	server.start(8000);

	connection clients[CLIENTS] = connection(ProtocolId, TimeOut);
	for(int i = 0; i < CLIENTS; i++)
        clients[i].start(8001 + i);

	for(int i = 0; i < CLIENTS; i++)
		clients[i].connect(myAddress);

    while(true)
    {
        for(int i = 0; i < CLIENTS; i++)
            clients[i].update(DeltaTime);

        server.update(DeltaTime);
        packets = true;
        while(packets)
        {
            if(server.receivePacket(16) == 0)
                packets = false;
        }

        for(int i = 0; i < CLIENTS; i++)
        {
            packets = true;
            while(packets)
            {
                if(clients[i].receivePacket(16) == 0)
                    packets = false;
            }
                clients[i].sendPacket(0,DeltaTime);
                server.sendPacket(i,DeltaTime);
        }

        waitsecs(DeltaTime);

    }//*/

    /*
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




		// detect changes in connection statedraw

		if (mode == Server && connected && !connection.isConnected())
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
        /*
		sendAccumulator += DeltaTime;

		while (sendAccumulator > 1.0f / sendRate)
		{
			unsigned char packet[PacketSize];
			memset(packet, 0, sizeof(packet));
			connection.sendPacket(packet, sizeof(packet));
			sendAccumulator -= 1.0f / sendRate;
		}draw

		while (true)
		{
			//unsigned char packet[256];
			int bytes_read = connection.receivePacket(256);
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

		waitsecs(DeltaTime);
	}

	getchar();

	ShutdownSockets();

	return 0;
	*/
	//return 0;
//}





