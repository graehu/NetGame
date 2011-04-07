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
    //mHeader = 16;
}

connection::~connection()
{
    if(mRunning)
        stop();
}

bool connection::start(int aPort)
{
    assert(!mRunning);
    mPort = aPort;
    mSendPacket.setAlloc(mSendPacket.getHeaderSize());
    mReceivePacket.setAlloc(mReceivePacket.getHeaderSize());
    printf("start connection on port %d\n", aPort);
    if (!mSocket.Open(aPort))
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

        mNewConnKeys.push_back(mKeyPool.back()); ///Receive key
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
    mNewConnKeys.push_back(mMailList.size()-1); /// Receive key
}


void connection::update(float deltaTime)
{
    assert(mRunning);

    bool accumRestart = false;

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

                /// i is this elements current position in the array, so if this was the first element
                /// i would be zero.

                ///Using writeData is a little hacky, but for now it will do.

				mSendPacket.writeData((unsigned short)(mProtocolId*2),0);
				mSendPacket.writeData((unsigned short)mMailList[i].second,2);
				mSendPacket.writeData((unsigned short)i,4);

                mSocket.Send(mMailList[i].first->mAddress, mSendPacket.getData(), 6);
				mSendPacket.clearPacket();
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
                mMailList[i].first->mFlow.Reset();
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
                mMailList[i].first->mFlow.Reset();
                mMailList[i].second = 0;
                mKeyPool.push_back(i);
                continue;
            }
        }
        mMailList[i].first->mStatistics.Update(deltaTime);
    }

}

bool connection::sendPacket(unsigned short aKey, float aDeltaTime)
{

    ///it would be cool to somehow add FlowControl to this function.

    assert(mRunning);

    if (mMailList.size() == 0 )
        return false;

    if (mMailList.size() <= aKey)
        return false;

    if (mMailList[aKey].first->mAddress.getAddress() == 0 )
        return false;

    if( mMailList[aKey].first->mState != eConnected)
        return false;
	///old way of doing things
    mMailList[aKey].first->mFlow.Update(aDeltaTime, mMailList[aKey].first->mStatistics.GetRoundTripTime() * 1000.0f);

    mMailList[aKey].first->mSendAccumulator += aDeltaTime;

    float sendRate = mMailList[aKey].first->mFlow.GetSendRate();
    float sendAccumulator = mMailList[aKey].first->mSendAccumulator;
    bool error = 0;

    if(sendAccumulator > (1.0f/sendRate))
    {

    mMailList[aKey].first->mSendAccumulator = 0;

    //sexy new way (no need for a local packet!)
	mSendPacket.writeProtocolId(mProtocolId); ///short
	mSendPacket.writeKey(mMailList[aKey].second); /// send key ///short
	mSendPacket.writeSequence(mMailList[aKey].first->mStatistics.GetLocalSequence()); /// sequence
	mSendPacket.writeAck(mMailList[aKey].first->mStatistics.GetRemoteSequence()); /// ack
	mSendPacket.writeAckBits(mMailList[aKey].first->mStatistics.GenerateAckBits()); /// acked bits
	mMailList[aKey].first->mStatistics.PacketSent(mSendPacket.getAlloc());

	error = mSocket.Send(mMailList[aKey].first->mAddress, mSendPacket.getData(), mSendPacket.getEnd());
	mSendPacket.clearPacket(); /// clears the packet so as you can start pushing data in from the start again
	//printf("sending packet\n");
    }

	return error;
}


int connection::receivePacket(unsigned int aSize)
{
	////////////////////////////////////////////////////////////////
	/// NOTE													 ///
	/// nothing is done with the data that is Received as of yet.///
	////////////////////////////////////////////////////////////////

    assert(mRunning);
    address nSender;

    mReceivePacket.clearPacket();
	mReceivePacket.setAlloc(aSize);
	unsigned char* packet = mReceivePacket.getData();
	// push this directly into mReceivePacket.
    int bytes_read = mSocket.Receive(nSender, packet, aSize);

    if (bytes_read == 0)
        return 0;

    mReceivePacket.setEnd(mReceivePacket.getHeaderSize()); /// only reading the mHeader for now

    unsigned short security = 0;

    security = mReceivePacket.readProtocolId();

    if((security != mProtocolId) && (security != (unsigned short)(mProtocolId*2)))
        return 0;

	bool initPacket = false;
    if(security == (unsigned short)(mProtocolId*2))
        initPacket = true;

    ///UNDER HEAVY DEVELOPMENT/////////////////////////////////////////////////////////////////////////////

    if(initPacket)
    {
        //printf("Received init packet \n");
        security = mReceivePacket.readUShort(2); ///the key they think i sent them.

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
                /// then check if it is sending the Receive key correctly
                security = mReceivePacket.readUShort(4);
                mMailList[mNewConnKeys[i]].second = security;

                security = mReceivePacket.readUShort(2);

                if(security == mNewConnKeys[i])
                {
                    ///printf("client connected1\n");

                    printf
                    (
                        "Port: %i Connected to: %d.%d.%d.%d:%d\n",
                        mPort,
                        nSender.getA(),
                        nSender.getB(),
                        nSender.getC(),
                        nSender.getD(),
                        nSender.getPort()
                    );

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
            security = mReceivePacket.readUShort(4);
            mMailList[mKeyPool.back()].first->mState = eConnecting;
            mMailList[mKeyPool.back()].first->mAddress = nSender;
            mMailList[mKeyPool.back()].first->mTimeoutAccumulator = 0;
            mMailList[mKeyPool.back()].first->mStatistics.Reset();
            mMailList[mKeyPool.back()].second = security;

            mNewConnKeys.push_back(mKeyPool.back());
            mKeyPool.pop_back();

            return 0;
        }
        /// printf("client connecting for first time \n");

        security = mReceivePacket.readUShort(4);

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

        security = mReceivePacket.readUShort(2);

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
                    /// then check if it is sending the Receive key correctly

                    //printf("send + 1: %d \n", (int)test);
                    //printf("Receive + 1: %d \n", (int)security);

                    if(security == mNewConnKeys[i])
                    {

                        printf
                        (
                            "Port: %i Connected to: %d.%d.%d.%d:%d\n",
                            mPort,
                            nSender.getA(),
                            nSender.getB(),
                            nSender.getC(),
                            nSender.getD(),
                            nSender.getPort()
                        );

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
        }



	connected = true; //legacy
    /// unsigned short sendKey = mReceivePacket.readUShort(2); /// removed because i don't think it's needed anymore
    unsigned int packet_sequence = mReceivePacket.readUInteger(4);
    unsigned int packet_ack = mReceivePacket.readUInteger(8);
    unsigned int packet_ack_bits = mReceivePacket.readUInteger(12);

    mMailList[security].first->mStatistics.PacketReceived(packet_sequence, bytes_read - 16); ///WHY DOES THIS SAY 14?!
    mMailList[security].first->mStatistics.ProcessAck(packet_ack, packet_ack_bits);
    mMailList[security].first->mTimeoutAccumulator = 0;

    //memcpy(data, &packet[mHeader], bytes_read - mHeader);

    return bytes_read;
    }


    return 0;
}
