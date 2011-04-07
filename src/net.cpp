#include "header/net.h"
#include <cstdlib>

using namespace net;

network::network(unsigned short protocolId, float timeout , unsigned int max_sequence) :
    connection(protocolId, timeout, max_sequence)
{

    mType = eClient;
    mPacketSize = 256; ///bad, should be passed in constructor or something.
}

network::~network()
{
}


bool network::init(netType type, int port)
{
    mReceivePacket.setDefinitions(&mDefines);
    mSendPacket.setDefinitions(&mDefines);

    /// packet1 update ///updating is the mooooost important.
    mDefines.beginPacket();
    mDefines.pushType((unsigned short)0); /// access key
    mDefines.pushType((int)0);            /// posx
    mDefines.pushType((int)0);            /// posy
    mDefines.pushType((int)0);            /// velox
    mDefines.pushType((int)0);            /// veloy
    mDefines.pushType((unsigned char)0);  /// commands
    mDefines.endPacket();
    ///packet2 playerUpdate /// the button presses.
    mDefines.beginPacket();
    mDefines.pushType((unsigned short)0); /// access key
    mDefines.pushType((unsigned char)0);  /// commands
    mDefines.endPacket();
    /// packet3 request ///second most important... in third place
    mDefines.beginPacket();
    mDefines.pushType((unsigned short)0); /// access key
    mDefines.pushType((unsigned short)0); /// request type ///check if it exists before packing.
    mDefines.endPacket();

	if (!start(port))
	{
		printf("could not start connection on port %d\n", port);
		return 1;
	}

	if(type == eClient)
	{
	    mType = eClient;
	    entity* me = new entity;
	    mEntities.push_back(me);

        address myAddress(127,0,0,1,8000);
        connect(myAddress);
        mEnUpdate.push_back(vector<enInfo>());
        mEnUpdate[0].push_back(enInfo(0,eUpdating));
        ///telling the server about myself
	}

	if(type == eServer)
	{
	    mType = eServer;
        //entity* me = new entity;
	    //mEntities.push_back(me);
	}



    return false;
}

bool network::update(float aDeltaTime)
{
    connection::update(aDeltaTime);

    while(receivePacket(mPacketSize) != 0)
    {
        //printf("oh dear god, the horror\n");
        ///first the connection checks if it has any packets to read
        mReceivePacket.setEnd(mPacketSize);
        mReceivePacket.analysePacket();

        unsigned short packetSender = mReceivePacket.readKey();
        unsigned short offset = mReceivePacket.getHeaderSize();
        unsigned short enType = 0;
        unsigned short accessKey = 0;
        unsigned char commands = 0;
        unsigned int j = 0;
        int xvelocity = 0;
        int yvelocity = 0;
        int xpos = 0;
        int ypos = 0;
        bool data = true;

        /// //////////////////////
        /// Packet Read Section///
        /// //////////////////////

        while(data)
        {
            //printf("curious\n");
            switch(mReceivePacket.getSectionType(j))
            {
                case 0:
                    ///empty
                    data = false;
                    break;
                case 1:
                    /// update
                    //mReceivePacket.getMembData(j,1,enType);

                    mReceivePacket.getMembData(j,1,accessKey);

                    initEntity(packetSender, accessKey);

                    if (mType == eClient)
                    {
                    xpos = mReceivePacket.readInteger(offset+4);
                    ypos = mReceivePacket.readInteger(offset+8);
                    xvelocity = mReceivePacket.readInteger(offset+12);
                    yvelocity = mReceivePacket.readInteger(offset+16);
                    mEntities[mEnUpdate[packetSender][accessKey].mEnKey]->setXPos(xpos);
                    mEntities[mEnUpdate[packetSender][accessKey].mEnKey]->setYPos(ypos);
                    mEntities[mEnUpdate[packetSender][accessKey].mEnKey]->setXVelocity(xvelocity);
                    mEntities[mEnUpdate[packetSender][accessKey].mEnKey]->setYVelocity(yvelocity);
                    }
                    //else{printf("do i want to see this... hmm\n");}
                    break;
                case 2:
                    ///update
                    mReceivePacket.getMembData(j,1,accessKey);

                    initEntity(packetSender, accessKey);

                    if (mType == eServer)
                    {
                    mReceivePacket.getMembData(j, 2, commands);
                    mEntities[mEnUpdate[packetSender][accessKey].mEnKey]->setCommands(commands);
                    //mEntities[mEnUpdate[packetSender][accessKey].mEnKey]->move();

                    }
                    break;
            }
            j++;
            offset = mReceivePacket.getSectionStart(j);

        }
    }
        /// ///////////////////////
        /// Packet Write Section///
        /// ///////////////////////

    for(int i = 0; i < mEnUpdate.size(); i++)
    {
        for(int ii = 0; ii < mEnUpdate[i].size(); ii++)
        {
            /// check if entity is initialised, if so, send update
            /// packet
            if(mEnUpdate[i][ii].mState == eUpdating)
            {
                if(mType == eServer)
                {
                    mSendPacket.beginPacketData(1);
                    mSendPacket.safePushData((unsigned short)ii);
                    mSendPacket.safePushData((int)mEntities[mEnUpdate[i][ii].mEnKey]->getXPos());
                    mSendPacket.safePushData((int)mEntities[mEnUpdate[i][ii].mEnKey]->getYPos());
                    mSendPacket.safePushData((int)mEntities[mEnUpdate[i][ii].mEnKey]->getXVelocity());
                    mSendPacket.safePushData((int)mEntities[mEnUpdate[i][ii].mEnKey]->getYVelocity());
                    mSendPacket.safePushData((unsigned char)mEntities[mEnUpdate[i][ii].mEnKey]->getCommands());
                    mSendPacket.endPacketData();

                }

                if(mType == eClient && ii == 0)
                {
                    mSendPacket.beginPacketData(2);
                    mSendPacket.safePushData((unsigned short)ii);
                    mSendPacket.safePushData((unsigned char)mEntities[mEnUpdate[i][ii].mEnKey]->getCommands());
                    mSendPacket.endPacketData();
                }
            }

            // if(mEnUpdate[i][ii].mState == eUninitialised)
        }
        /// need REAL delta time for packet...
        //if(mType == eServer)
        //{
        //    printf("Sending\n");
        //}
        mSendPacket.pushData((unsigned short)0);
        sendPacket(i, aDeltaTime);
        mSendPacket.clearPacket();
    }

    for(int i = 0; i < mEntities.size(); i++)
        mEntities[i]->move();

	return false;

}

void network::initEntity(unsigned short packetSender, unsigned short accessKey)
{
/// /////////////////////////////////////////////////////////
/// make sure that there is an update slot for this sender///
/// and push back                                         ///
/// /////////////////////////////////////////////////////////
while(packetSender >= mEnUpdate.size()) /// if i don't have a place for this sender, make one.
{
    mEnUpdate.push_back(vector<enInfo>());
}
/// ////////////////////////////////////////////////////
/// now i want to check if i have this entity or not.///
/// ////////////////////////////////////////////////////

if(accessKey == mEnUpdate[packetSender].size())
{

    /// the important thing to realise is that you are trying to keep each packet senders
    /// list the same on BOTH sides, so, the servers access key will be the same as the clients
    /// the only thing that should change is the entity id.

    /// i want him to know about all entities, for now so see how many entities there are
    /// and push that many onto his unknown list.


    entity* nEntity = new entity;
    mEntities.push_back(nEntity);
    if(mPort == 8005)printf("8005 initialising entity\n");
    mEnUpdate[packetSender].push_back(enInfo((mEntities.size()-1),eUpdating));
    //if(mType == eServer)

    for(int i = 0; i < mEntities.size(); i++)
    {
        if(i >= mEnUpdate[packetSender].size())
        {
            mEnUpdate[packetSender].push_back(enInfo(i,eUpdating));
            printf("pushing back entity to be initialised\n");
        }

    }

    /// this part should change if you don't want to tell everybody about this new entity
    /// but i do.

    /// ok so, someone new comes in, number one
    /// all people from now on will be told about number one.

    for(int i = 0; i < mEnUpdate.size(); i++)
    {
        if(i != packetSender)
        {
            mEnUpdate[i].push_back(enInfo((mEntities.size()-1), eUpdating));
        }
    }

    /// and now tell the sender about everyone else.

}
}

void network::draw()
{
    myRender.beginScene();
    for(int i = 0; i < mEntities.size(); i++)
    {
        myRender.draw(mEntities[i]->getXPos(),
                      mEntities[i]->getYPos(),
                      mEntities[i]->getXVelocity(),
                      mEntities[i]->getYVelocity());
    }
    myRender.endScene();
}

entity* network::getEntity(unsigned int element)
{
    if(element < mEntities.size())
        return mEntities[element];

    return 0;
}

void network::addEntity(void)
{

    entity* nEntity = new entity;
    mEntities.push_back(nEntity);
    //mEnUpdate[packetSender].push_back(enInfo((mEntities.size()-1),eInitialised));

    for(int i = 0; i < mEntities.size(); i++)
    {
        mEnUpdate[i].push_back(enInfo((mEntities.size()-1),eUninitialised));
    }

    printf("%i new entity added!\n", mPort);
}

