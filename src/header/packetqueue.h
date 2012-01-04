#ifndef PACKETQUEUE_H
#define PACKETQUEUE_H
// packet queue to store information about sent and received packets sorted in sequence order
//  + we define ordering using the "sequence_more_recent" function, this works provided there is a large gap when sequence wrap occurs
#include <list>
#include <assert.h>

namespace net
{

struct packetData
{
	unsigned int m_sequence;			// packet sequence number
	float m_time;					    // time offset since packet was sent or received (depending on context)
	int m_size;						// packet size in bytes
};

inline bool sequence_more_recent(unsigned int _s1, unsigned int _s2, unsigned int _maxSequence)
{
	return ((( _s1 > _s2 ) && ( _s1 - _s2 <= _maxSequence/2 )) || (( _s2 > _s1 ) && ( _s2 - _s1 > _maxSequence/2 )));
}		

class packetQueue : public std::list<packetData>
{
public:
		
	bool exists(unsigned int sequence);
	void insert_sorted(const packetData & _p, unsigned int _maxSequence);
	void verify_sorted(unsigned int _maxSequence);
};

}
#endif//PACKETQUEUE_H
