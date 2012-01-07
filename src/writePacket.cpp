#include "header/writePacket.h"
using namespace net;

bool writePacket::pushData(const unsigned char _data[], unsigned int _size)
{
	if((m_end + _size) > m_alloc)
	{
		unsigned char* temp;
		temp = m_data;
		m_data = new unsigned char[(m_end + _size)*2];
		if(temp != 0)
		{
            memcpy(&m_data[0], temp, m_end);
            delete temp;
		}
		m_alloc = (m_end + _size)*2;
	}

	memcpy(&m_data[m_end], _data, _size);
	m_end += _size;
	return false;
}


