#include "header/packetDef.h"
using namespace net;

packetDef::packetDef()
{
  m_packing = false;
  m_size = 0;
  //  m_ID = defRegister::instance().registerDefinition(this);
}

packetDef::~packetDef()
{

}

void packetDef::beginDefinition(void)
{
  if(!m_packing)
    {
      //the unsigned short that's being pushed back here is the packet type.
      //i.e. the identifier for this packetDef.
	  if(m_defintion.size() > 0)
		  m_defintion.clear();
      m_packing = true;
      //m_defintion.push_back(packType());

      //This will be the packetDef type in the future.
      //m_defintion.push_back(e_UShort);
      //m_size += sizeof(unsigned short);
      return;
    }
  if(m_packing){printf("packetDef: beginPacket(), error packet already begun\n"); return;}
};


void packetDef::endDefinition(void)
{
  if(!m_packing){printf("packetDef: endPacket(), error haven't begun, so can't end\n"); return;}
  else m_packing = false;
}

char* packetDef::getMembTypeName(unsigned int _member)
{

if(_member < m_defintion.size())
  {
	return defRegister::instance().getNameByType(m_defintion[_member]);
  }
  return 0;

}


