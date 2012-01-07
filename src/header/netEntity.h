#ifndef NETENTITY_H
#define NETENTITY_H

#include <vector>
#include "entity.h"

#include "packetDef.h"

namespace net
{

class netEntity : public entity
{
  	public:
		netEntity();
		~netEntity();

		//TODO turn this into an interface, virtual functions.
		//Or just overide... i dunno.
		void readPacket(unsigned char* _data);
		void writePacket(unsigned char* _data);
		unsigned short getDefSize(void){return m_define.getSize();}

		unsigned char getCommands(void){return m_command;}
		void setCommands(unsigned char command){m_command = command;}
		void move(void); //This should be gotten rid of :)

	protected:

	private:

		packetDef m_define;
    	unsigned char m_command;

};
}

#endif//NETENTITY_H

