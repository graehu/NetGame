#include "header/packetDef.h"
#include <cstdio>

packetDef::packetDef()
{
  mPacking = false;
  ///first define == empty packet.
  mPacketDefs.push_back(std::pair<std::vector<packType>, unsigned int>());
  mPacketDefs[0].first.push_back(eBool);
  mPacketDefs[0].second = 0;
  mPacketSizeAccumulator = 0;
}

packetDef::~packetDef()
{

}

void packetDef::beginPacket(void)
{
  if(!mPacking)
    {
      //the unsigned short that's being pushed back here is the packet type.
      //i.e. the identifier for this packetdef.
      mPacking = true;
      mPacketDefs.push_back(std::pair<std::vector<packType>, unsigned int>());
      //mPacketDefs.back().first.push_back((char*)typeid(unsigned short).name());
      mPacketDefs.back().first.push_back(eUShort);
      mPacketSizeAccumulator += sizeof(unsigned short);
      return;
    }
  if(mPacking){printf("packetDef: beginPacket(), error packet already begun\n"); return;}
};


void packetDef::endPacket(void)
{
  if(!mPacking){printf("packetDef: endPacket(), error haven't begun, so can't end\n"); return;}
  if(mPacking)
    {
      if(mPacketDefs.back().first.size() == 1 && mPacketDefs.size() > 0)
        {
	  //this happens in the event that nothing useful was pushed back.
	  //the definition is just abandoned.
	  mPacketDefs.pop_back();
	  mPacketSizeAccumulator = 0;
	  mPacking = false;
	  return;
        }
      else
        {
	  mPacketDefs.back().second = mPacketSizeAccumulator;
	  mPacketSizeAccumulator = 0;
	  mPacking = false;
	  return;
        }
    }
}

char* packetDef::getMembTypeName(unsigned int aDef, unsigned int aMember)
{
  if(aDef < mPacketDefs.size())
    {
    if(aMember < mPacketDefs[aDef].first.size())
      {
	return getNameByType(mPacketDefs[aDef].first[aMember]);
      }
    }

  //printf("packetDef: getMemberType(), error invalid arguments, out of bounds\n");
  return 0;
} /// which definition, then which member.


char* packetDef::getNameByType(packType aType)
{
  switch(aType)
    {
    case eBool:
      return (char*)typeid(bool).name();
    case eUChar:
      return (char*)typeid(unsigned char).name();
    case eChar:
      return (char*)typeid(char).name();
    case eUShort:
      return (char*)typeid(unsigned short).name();
    case eShort:
      return (char*)typeid(short).name();
    case eUInt:
      return (char*)typeid(unsigned int).name();
    case eInt:
      return (char*)typeid(int).name();
    case eFloat:
      return (char*)typeid(float).name();
    case eDouble:
      return (char*)typeid(double).name();
    }
  printf("packetDef: not knowing this type");
  assert(false);
}


unsigned int packetDef::getDefSize(unsigned int aDefID)
{
  if(aDefID < mPacketDefs.size())
    return mPacketDefs[aDefID].second;
  printf("packetDef: getDefSize(), error aDefID too high\n");
  return 0;
}

packType packetDef::getTypeByName(char* aName)
{
   ///find out how to replace this with a switch block
  if(aName == typeid(bool).name())
    return eBool;
  if(aName == typeid(unsigned char).name())
    return eUChar;
  if(aName == typeid(char).name())
    return eChar;
  if(aName == typeid(unsigned short).name())
    return eUShort;
  if(aName == typeid(short).name())
    return eShort;
  if(aName == typeid(unsigned int).name())
    return eUInt;
  if(aName == typeid(int).name())
    return eInt;
  if(aName == typeid(float).name())
    return eFloat;
  if(aName == typeid(double).name())
    return eDouble;
  printf("packetDef: not knowing this type please\n");
  assert(false); 
}

unsigned int packetDef::getTypeSizeByPackType(packType aType)
{

  switch(aType)
    {
    case eBool:
      return sizeof(bool);
    case eUChar:
      return sizeof(unsigned char);
    case eChar:
      return sizeof(char);
    case eUShort:
      return sizeof(unsigned short);
    case eShort:
      return sizeof(short);
    case eUInt:
      return sizeof(unsigned int);
    case eInt:
      return sizeof(int);
    case eFloat:
      return sizeof(float);
    case eDouble:
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
