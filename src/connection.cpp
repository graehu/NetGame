#include "header/connection.h"

using namespace net;
using namespace std;

connection::connection(unsigned short aProtocolId, float aTimeout, unsigned int aMax_sequence)
{
    mProtocolId = aProtocolId;
    mTimeout = aTimeout;
    mMax_sequence = aMax_sequence;
    mRunning = false;
    connected = false;
    mMailList.clear();
    mSendAccumulator = 0;
}

connection::~connection()
{
    if(mRunning)
        stop();
}

bool connection::start(int nPort)
{
    assert(!mRunning);
    printf("start connection on port %d\n", nPort);
    if (!mSocket.Open(nPort))
        return false;
    mRunning = true;
    return true;
}

void connection::stop()
{
    assert(mRunning);
    printf("stop connection\n");
    mMailList.empty();
    mSocket.Close();
    mRunning = false;
}

void connection::connect(const address & nAddress)
{

    printf("client trying: %d.%d.%d.%d:%d\n",
           nAddress.getA(),
           nAddress.getB(),
           nAddress.getC(),
           nAddress.getD(),
           nAddress.getPort()
          );

    if(!mKeyPool.empty())   /// If the key pool isn't empty
    {
        /// access mailList based on the key pools returned value
        /// and set mailList element up with the new connections
        /// data
        mMailList[mKeyPool.back()].first->mState = eConnecting;
        mMailList[mKeyPool.back()].first->mAddress = nAddress;
        mMailList[mKeyPool.back()].first->mTimeoutAccumulator = 0.0f;
        mMailList[mKeyPool.back()].first->mStatistics.Reset();
        mMailList[mKeyPool.back()].second = 0;


        /// add the key onto newConKeys and pop it off of the keyPool
        /// as it is now in use.

        mNewConnKeys.push_back(mKeyPool.back()); ///recieve key
        mKeyPool.pop_back();

        return;
    }

    /// if the key pool Is empty
    /// allocate a new sender
    /// initialise it as a new connection
    /// push it back onto mailList
    /// then push it's array position onto newConKeys

    sender* nMailer = new sender(mMax_sequence);
    nMailer->mAddress = nAddress;
    nMailer->mState = eConnecting;
    nMailer->mTimeoutAccumulator = 0.0f;
    unsigned short sendKey = 0;
    mMailList.push_back(std::pair<sender*, unsigned short>(nMailer, sendKey));
    mNewConnKeys.push_back(mMailList.size()-1); /// recieve key
}


void connection::update(float deltaTime)
{
    assert(mRunning);

    if(mMailList.size() == 0)
        return;
    if(mSendAccumulator > 1.0f / 30.0f)  /// send if sendAccumulator is greater than 1/30th,
            mSendAccumulator = 0;
    mSendAccumulator += deltaTime; /// advance the sendAccumulator by deltaTime, important for sending connection packets


    for(unsigned int i = 0; i < mMailList.size(); i++)  /// loop through all connections
    {
        if(mMailList[i].first->mState == eDisconnected)
            continue;

        mMailList[i].first->mTimeoutAccumulator += deltaTime;


        /// ///////////////////////////////////////////////////////////////////////////////////////////
        /// ///////////////////////////////////////////////////////////////////////////////////////////
        /// ///////////////////////////////////////////////////////////////////////////////////////////

        /// SECTION OF INTEREST
        /// init packet sending

        if(mMailList[i].first->mState == eConnecting) /// send a key for them
        {
            if(mSendAccumulator > 1.0f / 30.0f)  ///send if sendAccumulator is greater than 1/30th,
            {
                ///so roughly 30 a second if send accumulator is a fraction of a second
                mSendAccumulator = 0;
                //unsigned char packet[6];
                //unsigned short sendKey = mMailList[i].second;

                /// i is this elements current position in the array, so if this was the first element
                /// i would be zero.

                //writeInit(packet, (unsigned short)i, (unsigned short)sendKey);
				//mPacket.clearPacket();

				mPacket.pushData((unsigned short)(mProtocolId*2));
				mPacket.pushData((unsigned short)i); //may want to make this an unsigned short
				mPacket.pushData((unsigned short)mMailList[i].second);

				/*void connection::writeInit(unsigned char * init, unsigned short sendKey, unsigned short recieveKey)
				{
					mPacket.writeShort(init, mProtocolId*2); /// protocolId*2 this is so as we can differentiate between an init packet and a normal packet
					mPacket.writeShort(init + 2, sendKey);
					mPacket.writeShort(init + 4, recieveKey);
				}*/


				//printf("size: %i\n",mPacket.getSize());

                mSocket.Send(mMailList[i].first->mAddress, mPacket.getData(), 6);
				mPacket.clearPacket();
            }
        }

        /// ///////////////////////////////////////////////////////////////////////////////////////////
        /// ///////////////////////////////////////////////////////////////////////////////////////////
        /// ///////////////////////////////////////////////////////////////////////////////////////////


        if (mMailList[i].first->mTimeoutAccumulator > mTimeout)
        {
            if (mMailList[i].first->mState == eConnecting)
            {
                printf
                (
                    "Timed-out connecting to: %d.%d.%d.%d:%d\n",
                    mMailList[i].first->mAddress.getA(),
                    mMailList[i].first->mAddress.getB(),
                    mMailList[i].first->mAddress.getC(),
                    mMailList[i].first->mAddress.getD(),
                    mMailList[i].first->mAddress.getPort()
                );

                mMailList[i].first->mState = eDisconnected;
                mMailList[i].second = 0;

                for(unsigned int ii = 0; ii < mNewConnKeys.size(); ii++)
                {
                    if(mMailList[i].first->mAddress == mMailList[mNewConnKeys[ii]].first->mAddress) ///Removing a connection
                    {

                        if(mNewConnKeys[ii] != mNewConnKeys.back())
                        {
                            /// Switch the current connection out for
                            /// the connection at the back new connection
                            unsigned short temp;
                            temp = mNewConnKeys[ii];
                            mNewConnKeys[ii] = mNewConnKeys.back();
                            mNewConnKeys.back() = temp;
                        }

                        mNewConnKeys.pop_back();
                    }
                }

                /// push the connection onto the key pool because it can be reused
                mKeyPool.push_back(i);

                connected = false; ///this is bad, once i write the game class this will be out.
                continue;
            }
            else if ( mMailList[i].first->mState  == eConnected )
            {
                printf
                (
                    "Connection timed-out: %d.%d.%d.%d:%d\n",
                    mMailList[i].first->mAddress.getA(),
                    mMailList[i].first->mAddress.getB(),
                    mMailList[i].first->mAddress.getC(),
                    mMailList[i].first->mAddress.getD(),
                    mMailList[i].first->mAddress.getPort()
                );
                mMailList[i].first->mState = eDisconnected;
                mMailList[i].second = 0;
                mKeyPool.push_back(i);
                continue;
            }
        }
        mMailList[i].first->mStatistics.Update(deltaTime);
    }

}

bool connection::sendPacket( const unsigned char aData[], unsigned int aSize, unsigned short aKey)
{
    assert(mRunning);

    if (mMailList.size() == 0 )
        return false;

    if (mMailList.size() < aKey)
        return false;

    if (mMailList[aKey].first->mAddress.getAddress() == 0 )
        return false;

    if( mMailList[aKey].first->mState != eConnected)
        return false;

    unsigned int header = 16;
	///old way of doing things
    /*
	static unsigned int currentAlloc;

    static unsigned char* packet;

    ///make sure packet isn't null
    if(packet == NULL)
    {
         packet = new unsigned char[header+size];
         currentAlloc = header + size;
    }

    ///set the packet to be the highest value that ever comes into the function
    if (currentAlloc < (header + size))
    {
        delete packet;
        packet = new unsigned char[header+size];
    }

    unsigned int seq = mMailList[key].first->mStatistics.GetLocalSequence();
    unsigned int ack = mMailList[key].first->mStatistics.GetRemoteSequence();
    unsigned int ack_bits = mMailList[key].first->mStatistics.GenerateAckBits();

    writeHeader(packet, key, seq, ack, ack_bits);

    mMailList[key].first->mStatistics.PacketSent(size);

	memcpy(&packet[header], data, size);

	return mSocket.Send(mMailList[key].first->mAddress, packet, size + header);
	*/

	//sexy new way (no need for a local packet!)
	mPacket.pushData(mProtocolId);
	mPacket.pushData(aKey); //send key
	mPacket.pushData(mMailList[aKey].first->mStatistics.GetLocalSequence()); //sequence
	mPacket.pushData(mMailList[aKey].first->mStatistics.GetRemoteSequence()); //ack
	mPacket.pushData(mMailList[aKey].first->mStatistics.GenerateAckBits()); //acked bits
	mPacket.pushData(aData, aSize); //copy packet data
	mMailList[aKey].first->mStatistics.PacketSent(aSize);

	bool error = mSocket.Send(mMailList[aKey].first->mAddress, mPacket.getData(), aSize + header);
	mPacket.clearPacket(); //clears the packet so as you can start pushing data in from the start again
	//printf("sending packet2 \n");

	return error;
}


int connection::receivePacket(unsigned char data[], int size)
{
	////////////////////////////////////////////////////////////////
	/// NOTE													 ///
	/// nothing is done with the data that is recieved as of yet.///
	////////////////////////////////////////////////////////////////

    assert(mRunning);
    unsigned int header = 16;

	/////////////////////////////////////////////////////////////////////////////////////
	//I don't like this very much, I'd rather be putting data directly into mPacket.   //
	/////////////////////////////////////////////////////////////////////////////////////
	/*
    static unsigned int currentAlloc;
    static unsigned char* packet;

    ///make sure packet isn't null
    if(packet == NULL)
    {
         packet = new unsigned char[header+size];
         currentAlloc = header + size;
    }
    ///set the packet to be the highest value that ever comes into the function
    if (currentAlloc < (header + size))
    {
        delete packet;
        packet = new unsigned char[header+size];
    }
	*/
	/////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////

    address nSender;
	mPacket.setAlloc(size+header);
	unsigned char* packet = mPacket.getData(); //this method doesn't work so well for some reason...
	// push this directly into pacman.
    int bytes_read = mSocket.Receive(nSender, packet, size + header);

    if (bytes_read == 0)
        return 0;

    unsigned short security = 0;

    mPacket.readShort(&packet[0], security);

    if((security != mProtocolId) && (security != (unsigned short)(mProtocolId*2)))
        return false;

	bool initPacket = false;
    if(security == (unsigned short)(mProtocolId*2))
        initPacket = true;

    ///UNDER HEAVY DEVELOPMENT/////////////////////////////////////////////////////////////////////////////

    if(initPacket)
    {
        printf("recieved init packet \n");
        mPacket.readShort(&packet[4], security); ///the key they think i sent them.

            if(security < mMailList.size())
            {
                if(mMailList[security].first->mAddress == nSender)
                {
                    if(mMailList[security].first->mState == eConnected)
                    {
                        /// the person sending this init packet is already connected
                        /// so don't perform init operations on this address.
                            return 0;
                    }
                }
            }

        ///checks if the sender is in new connections or not

        for(unsigned int i = 0; i < mNewConnKeys.size(); i++)
        {

            if(mMailList[mNewConnKeys[i]].first->mAddress == nSender)
            {
                /// Look through all the new connections until you find
                /// the one that belongs to this sender
                /// set his sent key to what it's telling you
                /// then check if it is sending the recieve key correctly
                mPacket.readShort(&packet[2], security);
                mMailList[mNewConnKeys[i]].second = security;

                mPacket.readShort(&packet[4], security);

                if(security == mNewConnKeys[i])
                {
                    printf("client connected1\n");
					/// judging by my output this could be a bad place for these lines.
                    mMailList[security].first->mState = eConnected;
                    connected = true;

                    if(mNewConnKeys[i] != mNewConnKeys.back())
                    {
                        /// Switch the current connection out for
                        /// the connection at the back new connection
                        unsigned short temp;
                        temp = mNewConnKeys[i];
                        mNewConnKeys[i] = mNewConnKeys.back();
                        mNewConnKeys.back() = temp;
                    }
                    mNewConnKeys.pop_back();
                }
                return 0;
            }
        }


        ///if it is a new connection then give it a key if there is one spare in the key pool
        if(!mKeyPool.empty())
        {
            mPacket.readShort(&packet[2], security);
            mMailList[mKeyPool.back()].first->mState = eConnecting;
            mMailList[mKeyPool.back()].first->mAddress = nSender;
            mMailList[mKeyPool.back()].first->mTimeoutAccumulator = 0;
            mMailList[mKeyPool.back()].first->mStatistics.Reset();
            mMailList[mKeyPool.back()].second = security;

            mNewConnKeys.push_back(mKeyPool.back());
            mKeyPool.pop_back();

            return 0;
        }
        printf("client connecting for first time \n");

        mPacket.readShort(&packet[2], security);

        sender* nMailer = new sender(mMax_sequence);
        nMailer->mAddress = nSender;
        nMailer->mState = eConnecting;
        nMailer->mTimeoutAccumulator = 0.0f;
        nMailer->mStatistics.Reset();

        mMailList.push_back(std::pair<sender*, unsigned short>(nMailer, security));
        mNewConnKeys.push_back(mMailList.size()-1);


    }
    else
    {
        ///note two is a different thing if it's not an init packet
        ///it's the access key in this case.
        mPacket.readShort(&packet[2], security);

        if(security > mMailList.size())
            return 0;

        if(mMailList[security].first->mState == eConnecting)
        {
            for(unsigned int i = 0; i < mNewConnKeys.size(); i++)
            {
                if(mMailList[mNewConnKeys[i]].first->mAddress == nSender)
                {
                    /// Look through all the new connections until you find
                    /// the one that belongs to this sender
                    /// then check if it is sending the recieve key correctly

                    //printf("send + 1: %d \n", (int)test);
                    //printf("recieve + 1: %d \n", (int)security);

                    if(security == mNewConnKeys[i])
                    {
                        printf("client connected2\n");
                        mMailList[security].first->mState = eConnected;
                        connected = true;
                        ///by this point i must've sent the right key to them

                        if(mNewConnKeys[i] != mNewConnKeys.back())
                        {
                            /// Switch the current connection out for
                            /// the connection at the back new connection
                            unsigned short temp;
                            temp = mNewConnKeys[i];
                            mNewConnKeys[i] = mNewConnKeys.back();
                            mNewConnKeys.back() = temp;
                        }
                        mNewConnKeys.pop_back();
                    }
                }
            }
            //printf("do we get here? \n");
        }

	connected = true; //legacy
    unsigned short sendKey = 0;
    unsigned int packet_sequence = 0;
    unsigned int packet_ack = 0;
    unsigned int packet_ack_bits = 0;
    readHeader(packet, sendKey, packet_sequence, packet_ack, packet_ack_bits);

    mMailList[security].first->mStatistics.PacketReceived(packet_sequence, bytes_read - 14);
    mMailList[security].first->mStatistics.ProcessAck(packet_ack, packet_ack_bits);
    mMailList[security].first->mTimeoutAccumulator = 0;


    memcpy(data, &packet[header], bytes_read - header);

    return bytes_read - header;
    }

    return 1;
}

/// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///Utility Functions


void connection::writeInit(unsigned char * init, unsigned short sendKey, unsigned short recieveKey)
{
    mPacket.writeShort(init, mProtocolId*2); /// protocolId*2 this is so as we can differentiate between an init packet and a normal packet
    mPacket.writeShort(init + 2, sendKey);
    mPacket.writeShort(init + 4, recieveKey);
}

void connection::readInit(const unsigned char * init, unsigned short & sendKey, unsigned short & recieveKey)
{
    mPacket.readShort(init + 2, sendKey);
    mPacket.readShort(init + 4, recieveKey);
}

void connection::writeHeader(unsigned char * header, unsigned short  sendKey, unsigned int sequence, unsigned int ack, unsigned int ack_bits)
{
    mPacket.writeShort(header, mProtocolId);
    mPacket.writeShort(header + 2, sendKey);
    mPacket.writeInteger(header + 4, sequence);
    mPacket.writeInteger(header + 8, ack);
    mPacket.writeInteger(header + 12, ack_bits);
}


void connection::readHeader(const unsigned char * header, unsigned short & sendKey, unsigned int & sequence, unsigned int & ack, unsigned int & ack_bits)
{
    mPacket.readShort(header + 2, sendKey);
    mPacket.readInteger(header + 4, sequence);
    mPacket.readInteger(header + 8, ack);
    mPacket.readInteger(header + 12, ack_bits);
}
