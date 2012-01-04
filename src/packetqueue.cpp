#include "header/packetqueue.h"
using namespace net;

		
bool packetQueue::exists( unsigned int sequence )
{
	for ( iterator itor = begin(); itor != end(); ++itor )
		if ( itor->m_sequence == sequence )
			return true;
	return false;
}
		
void packetQueue::insert_sorted( const packetData & _p, unsigned int _maxSequence )
{
	if ( empty() )
	{
		push_back( _p );
	}
	else
	{
		if ( !sequence_more_recent( _p.m_sequence, front().m_sequence, _maxSequence ) )
		{
			push_front( _p );
		}
		else if ( sequence_more_recent( _p.m_sequence, back().m_sequence, _maxSequence ) )
		{
			push_back( _p );
		}
		else
		{
			for ( packetQueue::iterator itor = begin(); itor != end(); itor++ )
			{
				assert( itor->m_sequence != _p.m_sequence );
				if ( sequence_more_recent( itor->m_sequence, _p.m_sequence, _maxSequence ) )
				{
					insert( itor, _p );
					break;
				}
			}
		}
	}
}
		
void packetQueue::verify_sorted( unsigned int max_sequence )
{
	packetQueue::iterator prev = end();
	for ( packetQueue::iterator itor = begin(); itor != end(); itor++ )
	{
		assert( itor->m_sequence <= max_sequence );
		if ( prev != end() )
		{
			assert( sequence_more_recent(itor->m_sequence, prev->m_sequence, max_sequence));
			prev = itor;
		}
	}
}
