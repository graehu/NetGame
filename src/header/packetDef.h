#ifndef PACKETDEF_H
#define PACKETDEF_H

#include <typeinfo>
#include <vector>
#include <cstdio>
#include <cassert>

enum packType
  {
    eBool = 0,
    eUChar,
    eChar,
    eUShort,
    eShort,
    eUInt,
    eInt,
    eFloat,
    eDouble
  };

class packetDef
{
 public:

  packetDef();
  ~packetDef();

  void beginPacket(void);
  template <typename T>
    bool pushType(T aType)
    {
      if(mPacking)
	{
	  packType myType;
	  myType = getTypeByName((char*)typeid(aType).name());
	  mPacketDefs.back().first.push_back(myType);
	  mPacketSizeAccumulator += getTypeSizeByPackType(myType);
	  return false;
	}
      else
	{
	  printf("packetDef: pushType(), error need to beginPacket()\n");
	  return true;
	}
    }
  unsigned int getPacketDefID(void){assert(mPacking);return mPacketDefs.size()-1;}
  void endPacket(void);

  char* getMembTypeName(unsigned int aDef, unsigned int aMember);
  char* getNameByType(packType aType);
  packType getTypeByName(char* aName);
  unsigned int getTypeSizeByName(char* aName);
  unsigned int getTypeSizeByPackType(packType aType);
  unsigned int getDefSize(unsigned int aDefID);
  unsigned int size(){return mPacketDefs.size();}

 protected:

  bool mPacking;

 private:

  unsigned int mPacketSizeAccumulator;
  //std::vector<std::pair<std::vector<char*>, unsigned int> > mPacketDefs; /// types(name) and total size
  std::vector<std::pair<std::vector<packType>, unsigned int> > mPacketDefs; /// types(name) and total size
};


#endif // PACKETDEF_H
