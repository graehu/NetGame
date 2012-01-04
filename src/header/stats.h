#ifndef NETSTATS_H
#define NETSTATS_H

#include "packetqueue.h"
#include <stdio.h>
#include <vector>
// reliability system to support reliable connection
//  + manages sent, received, pending ack and acked packet queues

namespace net
{

  class stats
  {
  public:

    stats(unsigned int _maxSequence = 0xFFFFFFFF);
    void reset();
    void packetSent(int _size);
    void packetReceived(unsigned int _sequence, int _size);
    unsigned int generateAckBits();
    void processAck(unsigned int ack, unsigned int ack_bits);
    void update(float _deltaTime);
    void validate();

    // utility functions

    static bool sequence_more_recent(unsigned int _s1, unsigned int _s2, unsigned int _maxSequence);

    static int bit_index_for_sequence(unsigned int _sequence, unsigned int _ack, unsigned int _maxSequence);
    static unsigned int generate_ack_bits(unsigned int _ack, const packetQueue & _receivedQueue, unsigned int _maxSequence);
    static void process_ack(unsigned int _ack, unsigned int _ack_bits,
			    packetQueue & _pendingAckQueue, packetQueue & _ackedQueue,
			    std::vector<unsigned int> & _acks, unsigned int & _ackedPackets,
			    float & _rtt, unsigned int _maxSequence);

    // data accessors

    unsigned int getLocalSequence() const;
    unsigned int getRemoteSequence() const;
    unsigned int getMaxSequence() const;
    void getAcks(unsigned int ** acks, int & count);
    unsigned int getSentPackets() const;
    unsigned int getReceivedPackets() const;
    unsigned int getLostPackets() const;
    unsigned int getAckedPackets() const;
    float getSentBandwidth() const;
    float getAckedBandwidth() const;
    float getRoundTripTime() const;
    int getHeaderSize() const;

  protected:

    void advanceQueueTime(float _deltaTime);
    void updateQueues();
    void updateStats();

  private:

    unsigned int m_maxSequence;		/// maximum sequence value before wrap around (used to test sequence wrap at low # values)
    unsigned int m_localSequence;		/// local sequence number for most recently sent packet
    unsigned int m_remoteSequence;		/// remote sequence number for most recently received packet

    unsigned int m_sentPackets;		/// total number of packets sent
    unsigned int m_recvPackets;		/// total number of packets received
    unsigned int m_lostPackets;		/// total number of packets lost
    unsigned int m_ackedPackets;		/// total number of packets acked
    float m_sentBandwidth;			/// approximate sent bandwidth over the last second
    float m_ackedBandwidth;			/// approximate acked bandwidth over the last second
    float m_rtt;				/// estimated round trip time
    float m_rttMaximum;			/// maximum expected round trip time (hard coded to one second for the moment)

    std::vector<unsigned int> m_acks;		/// acked packets from last set of packet receives. cleared each update!
    packetQueue sentQueue;			/// sent packets used to calculate sent bandwidth (kept until rtt_maximum)
    packetQueue pendingAckQueue;		/// sent packets which have not been acked yet (kept until rtt_maximum * 2 )
    packetQueue receivedQueue;		/// received packets for determining acks to send (kept up to most recent recv sequence - 32)
    packetQueue ackedQueue;			/// acked packets (kept until rtt_maximum * 2)
  };

}
#endif//NETSTATS
