#ifndef WRITEPACKET_H
#define WRITEPACKET_H
#include "packet.h"
#include "dataUtils.h"
namespace net
{
class writePacket : public packet
{
public:

	bool pushData(const unsigned char _data[], unsigned int _size);

	//header stuffs.
    void writeProtocolId(unsigned short _ID){writeData(_ID,0);}
    void writeKey(unsigned short _key){writeData(_key,2);}
    void writeSequence(unsigned int _seq){writeData(_seq,4);}
    void writeAck(unsigned int _ack){writeData(_ack,8);}
    void writeAckBits(unsigned int _ackBits){writeData(_ackBits,12);}
    void writeHandleID(unsigned short _handleID){writeData(_handleID,16);}

	template <typename T>
	bool pushData(T _data)
	{
	    if(!m_packing){
		unsigned int size = sizeof(_data);
		if((m_end + size) > m_alloc)
		{
			unsigned char* temp;
			temp = m_data;
			m_data = new unsigned char[(m_end + size)*2];
            if(temp != 0)
            {
                memcpy(&m_data[0], temp, m_end);
                delete temp;
            }
			m_alloc = (m_end + size)*2;
		}
		writeData(_data, m_end);

		m_end += size;
		return false;}else return true;
	};

    ///generic data writer.
	template <typename T>
    bool writeData(T _type, unsigned int _offset)
	{
		unsigned int size = sizeof(_type);
		//TODO add assert here instead of printf()
		if((_offset + size) > m_alloc)
		{
		    printf("Packet: writeData() error, offset too large.\n");
		    return true;
		}
		dataUtils::instance().writeData(_type, &m_data[_offset]);
		return false;
	}
};
}
#endif//WRITEPACKET_H



/// code graveyard

/*
 *
		for(unsigned int i = 0; i < size; i++)
		{
			if (i > 0) m_data[_offset+i] = (_data >> (size-(i+1))*8);
			else       m_data[_offset+i] = ((_data >> (size-(i+1))*8) & 0xFF);
		}
 */
