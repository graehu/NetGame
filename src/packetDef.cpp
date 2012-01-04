#include "header/packetDef.h"
#include <cstdio>

packetDef::packetDef()
{
  m_packing = false;
  ///first define == empty packet.
  m_packetDefs.push_back(std::pair<std::vector<packType>, unsigned int>());
  m_packetDefs[0].first.push_back(e_bool);
  m_packetDefs[0].second = 0;
  m_packetSizeAccumulator = 0;
}

packetDef::~packetDef()
{

}

void packetDef::beginPacket(void)
{
  if(!m_packing)
    {
      //the unsigned short that's being pushed back here is the packet type.
      //i.e. the identifier for this packetdef.
      m_packing = true;
      m_packetDefs.push_back(std::pair<std::vector<packType>, unsigned int>());
      //mPacketDefs.back().first.push_back((char*)typeid(unsigned short).name());
      m_packetDefs.back().first.push_back(e_UShort);
      m_packetSizeAccumulator += sizeof(unsigned short);
      return;
    }
  if(m_packing){printf("packetDef: beginPacket(), error packet already begun\n"); return;}
};


void packetDef::endPacket(void)
{
  if(!m_packing){printf("packetDef: endPacket(), error haven't begun, so can't end\n"); return;}
  if(m_packing)
    {
      if(m_packetDefs.back().first.size() == 1 && m_packetDefs.size() > 0)
        {
	  //this happens in the event that nothing useful was pushed back.
	  //the definition is just abandoned.
	  m_packetDefs.pop_back();
	  m_packetSizeAccumulator = 0;
	  m_packing = false;
	  return;
        }
      else
        {
	  m_packetDefs.back().second = m_packetSizeAccumulator;
	  m_packetSizeAccumulator = 0;
	  m_packing = false;
	  return;
        }
    }
}

char* packetDef::getMembTypeName(unsigned int _def, unsigned int _member)
{
  if(_def < m_packetDefs.size())
    {
    if(_member < m_packetDefs[_def].first.size())
      {
	return getNameByType(m_packetDefs[_def].first[_member]);
      }
    }

  //printf("packetDef: getMemberType(), error invalid arguments, out of bounds\n");
  return 0;
} /// which definition, then which member.


char* packetDef::getNameByType(packType _type)
{
  switch(_type)
    {
    case e_bool:
      return (char*)typeid(bool).name();
    case e_UChar:
      return (char*)typeid(unsigned char).name();
    case e_char:
      return (char*)typeid(char).name();
    case e_UShort:
      return (char*)typeid(unsigned short).name();
    case e_short:
      return (char*)typeid(short).name();
    case e_UInt:
      return (char*)typeid(unsigned int).name();
    case e_int:
      return (char*)typeid(int).name();
    case e_float:
      return (char*)typeid(float).name();
    case e_double:
      return (char*)typeid(double).name();
    }
  printf("packetDef: not knowing this type");
  assert(false);
  return (char*)typeid(int).name();
}


unsigned int packetDef::getDefSize(unsigned int aDefID)
{
  if(aDefID < m_packetDefs.size())
    return m_packetDefs[aDefID].second;
  printf("packetDef: getDefSize(), error aDefID too high\n");
  return 0;
}

packType packetDef::getTypeByName(char* aName)
{
   ///find out how to replace this with a switch block
  if(aName == typeid(bool).name())
    return e_bool;
  if(aName == typeid(unsigned char).name())
    return e_UChar;
  if(aName == typeid(char).name())
    return e_char;
  if(aName == typeid(unsigned short).name())
    return e_UShort;
  if(aName == typeid(short).name())
    return e_short;
  if(aName == typeid(unsigned int).name())
    return e_UInt;
  if(aName == typeid(int).name())
    return e_int;
  if(aName == typeid(float).name())
    return e_float;
  if(aName == typeid(double).name())
    return e_double;
  printf("packetDef: not knowing this type please\n");
  assert(false); 
}

unsigned int packetDef::getTypeSizeByPackType(packType aType)
{

  switch(aType)
    {
    case e_bool:
      return sizeof(bool);
    case e_UChar:
      return sizeof(unsigned char);
    case e_char:
      return sizeof(char);
    case e_UShort:
      return sizeof(unsigned short);
    case e_short:
      return sizeof(short);
    case e_UInt:
      return sizeof(unsigned int);
    case e_int:
      return sizeof(int);
    case e_float:
      return sizeof(float);
    case e_double:
      return sizeof(double);
    }

  printf("packetDef: not knowing this type please\n");
  assert(false);
}



unsigned int packetDef::getTypeSizeByName(char* aName)
{
  ///find out how to replace this with a switch block
  if(aName == typeid(bool).name())
    return sizeof(bool);
  if(aName == typeid(unsigned char).name())
    return sizeof(unsigned char);
  if(aName == typeid(char).name())
    return sizeof(char);
  if(aName == typeid(unsigned short).name())
    return sizeof(unsigned short);
  if(aName == typeid(short).name())
    return sizeof(short);
  if(aName == typeid(unsigned int).name())
    return sizeof(unsigned int);
  if(aName == typeid(int).name())
    return sizeof(int);
  if(aName == typeid(float).name())
    return sizeof(float);
  if(aName == typeid(double).name())
    return sizeof(double);
  printf("packetDef: not knowing this type please\n");
  assert(false);
}
