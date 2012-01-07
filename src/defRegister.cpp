#include "header/packetDef.h"
using namespace net;

defRegister::defRegister()
{
	packetDef* zeroDefinition = new packetDef();
	m_definitions.push_back(zeroDefinition);
}


packetDef& defRegister::getDef(unsigned int _defID)
{
	if(m_definitions.size() > _defID)
		return *m_definitions[_defID];
	else
	{
		printf("No definition for that value\n");
		assert(false);
	}
}


char* defRegister::getNameByType(packType _type)
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

packType defRegister::getTypeByName(char* aName)
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
  return e_int;
}

unsigned int defRegister::getTypeSizeByPackType(packType aType)
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
  return e_int;
}

unsigned int defRegister::getTypeSizeByName(char* aName)
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
  return sizeof(int);
}
