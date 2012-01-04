#include "header/stats.h"
using namespace net;

stats::stats(unsigned int _maxSequence)
{
  this->m_rttMaximum = m_rttMaximum;
  this->m_maxSequence = _maxSequence;

  reset();
}

void stats::reset()
{
  m_localSequence = 0;
  m_remoteSequence = 0;
  sentQueue.clear();
  receivedQueue.clear();
  pendingAckQueue.clear();
  ackedQueue.clear();
  m_sentPackets = 0;
  m_recvPackets = 0;
  m_lostPackets = 0;
  m_ackedPackets = 0;
  m_sentBandwidth = 0.0f;
  m_ackedBandwidth = 0.0f;
  m_rtt = 0.0f;
  m_rttMaximum = 1.0f;
}

void stats::packetSent(int _size)
{
  if (sentQueue.exists( m_localSequence))
    {
      printf("local sequence %d exists\n", m_localSequence);
      for (packetQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor)
	printf(" + %d\n", itor->m_sequence);
    }
  assert(!sentQueue.exists(m_localSequence));
  assert(!pendingAckQueue.exists(m_localSequence));
  packetData data;
  data.m_sequence = m_localSequence;
  data.m_time = 0.0f;
  data.m_size = _size;
  sentQueue.push_back(data);
  pendingAckQueue.push_back(data);
  m_sentPackets++;
  m_localSequence++;
  if (m_localSequence > m_maxSequence)
    m_localSequence = 0;
}

void stats::packetReceived(unsigned int _sequence, int _size)
{
  m_recvPackets++;
  if (receivedQueue.exists(_sequence))
    return;
  packetData data;
  data.m_sequence = _sequence;
  data.m_time = 0.0f;
  data.m_size = _size;
  receivedQueue.push_back(data);
  if (sequence_more_recent(_sequence, m_remoteSequence, m_maxSequence))
    m_remoteSequence = _sequence;
}

unsigned int stats::generateAckBits()
{
  return generate_ack_bits(getRemoteSequence(), receivedQueue, m_maxSequence);
}

void stats::processAck(unsigned int _ack, unsigned int _ackBits)
{
  process_ack(_ack, _ackBits, pendingAckQueue, ackedQueue, m_acks, m_ackedPackets, m_rtt, m_maxSequence);
}

void stats::update(float _deltaTime)
{
  m_acks.clear();
  advanceQueueTime(_deltaTime);
  updateQueues();
  updateStats();
#ifdef NET_UNIT_TEST
  validate();
#endif
}

void stats::validate()
{
  sentQueue.verify_sorted(m_maxSequence);
  receivedQueue.verify_sorted(m_maxSequence);
  pendingAckQueue.verify_sorted(m_maxSequence);
  ackedQueue.verify_sorted(m_maxSequence);
}

// utility functions
bool stats::sequence_more_recent(unsigned int _s1, unsigned int _s2, unsigned int _maxSequence)
{
  return ((( _s1 > _s2 ) && ( _s1 - _s2 <= _maxSequence/2 )) || (( _s2 > _s1 ) && ( _s2 - _s1 > _maxSequence/2 )));
}

int stats::bit_index_for_sequence( unsigned int _sequence, unsigned int _ack, unsigned int _maxSequence )
{
  assert(_sequence != _ack);
  assert(!sequence_more_recent(_sequence, _ack, _maxSequence));
  if(_sequence > _ack)
    {
      assert(_ack < 33);
      assert(_maxSequence >= _sequence);
      return _ack + (_maxSequence - _sequence);
    }
  else
    {
      assert(_ack >= 1);
      assert(_sequence <= _ack - 1);
      return _ack - 1 - _sequence;
    }
}

unsigned int stats::generate_ack_bits(unsigned int _ack, const packetQueue & _receivedQueue, unsigned int _maxSequence)
{
  unsigned int ack_bits = 0;
  for(packetQueue::const_iterator itor = _receivedQueue.begin(); itor != _receivedQueue.end(); itor++)
    {
      if(itor->m_sequence == _ack || sequence_more_recent(itor->m_sequence, _ack, _maxSequence))
	break;
      int bit_index = bit_index_for_sequence(itor->m_sequence, _ack, _maxSequence);
      if (bit_index <= 31)
	ack_bits |= 1 << bit_index;
    }
  return ack_bits;
}

void stats::process_ack(unsigned int _ack, unsigned int _ackBits,
			packetQueue & _pendingAckQueue, packetQueue & _ackedQueue,
			std::vector<unsigned int> & _acks, unsigned int & _ackedPackets,
			float & _rtt, unsigned int _maxSequence)
{
  if( _pendingAckQueue.empty() )
    return;

  packetQueue::iterator itor = _pendingAckQueue.begin();
  while(itor != _pendingAckQueue.end())
    {
      bool acked = false;

      if(itor->m_sequence == _ack)
	{
	  acked = true;
	}
      else if(!sequence_more_recent(itor->m_sequence, _ack, _maxSequence))
	{
	  int bit_index = bit_index_for_sequence(itor->m_sequence, _ack, _maxSequence);
	  if ( bit_index <= 31 )
	    acked = ( _ackBits >> bit_index ) & 1;
	}

      if(acked)
	{
	  _rtt += (itor->m_time - _rtt) * 0.1f;
	  _ackedQueue.insert_sorted(*itor, _maxSequence);
	  _acks.push_back(itor->m_sequence);
	  _ackedPackets++;
	  itor = _pendingAckQueue.erase(itor);
	}
      else
	++itor;
    }
}

// data accessors

unsigned int stats::getLocalSequence() const
{
  return m_localSequence;
}

unsigned int stats::getRemoteSequence() const
{
  return m_remoteSequence;
}

unsigned int stats::getMaxSequence() const
{
  return m_maxSequence;
}

void stats::getAcks( unsigned int ** acks, int & count )
{
  *acks = &this->m_acks[0];
  count = (int) this->m_acks.size();
}

unsigned int stats::getSentPackets() const
{
  return m_sentPackets;
}

unsigned int stats::getReceivedPackets() const
{
  return m_recvPackets;
}

unsigned int stats::getLostPackets() const
{
  return m_lostPackets;
}

unsigned int stats::getAckedPackets() const
{
  return m_ackedPackets;
}

float stats::getSentBandwidth() const
{
  return m_sentBandwidth;
}

float stats::getAckedBandwidth() const
{
  return m_ackedBandwidth;
}

float stats::getRoundTripTime() const
{
  return m_rtt;
}

int stats::getHeaderSize() const
{
  return 12;
}



void stats::advanceQueueTime(float _deltaTime)
{
  for(packetQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); itor++)
    itor->m_time += _deltaTime;
  for(packetQueue::iterator itor = receivedQueue.begin(); itor != receivedQueue.end(); itor++)
    itor->m_time += _deltaTime;
  for(packetQueue::iterator itor = pendingAckQueue.begin(); itor != pendingAckQueue.end(); itor++)
    itor->m_time += _deltaTime;
  for(packetQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); itor++)
    itor->m_time += _deltaTime;
}

void stats::updateQueues()
{
  const float epsilon = 0.001f;
  while(sentQueue.size() && sentQueue.front().m_time > m_rttMaximum + epsilon)
    sentQueue.pop_front();
  if(receivedQueue.size())
    {
      const unsigned int latest_sequence = receivedQueue.back().m_sequence;
      const unsigned int minimum_sequence = latest_sequence >= 34 ? (latest_sequence - 34) : m_maxSequence - (34 - latest_sequence);
      while(receivedQueue.size() && !sequence_more_recent(receivedQueue.front().m_sequence, minimum_sequence, m_maxSequence))
	receivedQueue.pop_front();
    }
  while(ackedQueue.size() && ackedQueue.front().m_time > m_rttMaximum * 2 - epsilon)
    ackedQueue.pop_front();
  while(pendingAckQueue.size() && pendingAckQueue.front().m_time > m_rttMaximum + epsilon)
    {
      pendingAckQueue.pop_front();
      m_lostPackets++;
    }
}

void stats::updateStats()
{
  int sent_bytes_per_second = 0;
  for (packetQueue::iterator itor = sentQueue.begin(); itor != sentQueue.end(); ++itor)
    sent_bytes_per_second += itor->m_size;
  int acked_packets_per_second = 0;
  int acked_bytes_per_second = 0;
  for (packetQueue::iterator itor = ackedQueue.begin(); itor != ackedQueue.end(); ++itor)
    {
      if(itor->m_time >= m_rttMaximum)
	{
	  acked_packets_per_second++;
	  acked_bytes_per_second += itor->m_size;
	}
    }
  sent_bytes_per_second /= m_rttMaximum;
  acked_bytes_per_second /= m_rttMaximum;
  m_sentBandwidth = sent_bytes_per_second * ( 8 / 1000.0f );
  m_ackedBandwidth = acked_bytes_per_second * ( 8 / 1000.0f );
}

