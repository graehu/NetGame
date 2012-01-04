#ifndef PACKETDEF_H
#define PACKETDEF_H

#include <typeinfo>
#include <vector>
#include <cstdio>
#include <cassert>

enum packType
  {
    e_bool = 0,
    e_UChar,
    e_char,
    e_UShort,
    e_short,
    e_UInt,
    e_int,
    e_float,
    e_double
  };

class packetDef
{
 public:

  packetDef();
  ~packetDef();

  void beginPacket(void);
  template <typename T>
    bool pushType(T _type)
    {
      if(m_packing)
	{
	  packType myType;
	  myType = getTypeByName((char*)typeid(_type).name());
	  m_packetDefs.back().first.push_back(myType);
	  m_packetSizeAccumulator += getTypeSizeByPackType(myType);
	  return false;
	}
      else
	{
	  printf("packetDef: pushType(), error need to beginPacket()\n");
	  return true;
	}
    }
  unsigned int getPacketDefID(void){assert(m_packing);return m_packetDefs.size()-1;}
  void endPacket(void);

  char* getMembTypeName(unsigned int _def, unsigned int _member);
  char* getNameByType(packType _type);
  packType getTypeByName(char* _name);
  unsigned int getTypeSizeByName(char* _name);
  unsigned int getTypeSizeByPackType(packType _type);
  unsigned int getDefSize(unsigned int _defID);
  unsigned int size(){return m_packetDefs.size();}

 protected:

  bool m_packing;

 private:

  unsigned int m_packetSizeAccumulator;
  //std::vector<std::pair<std::vector<char*>, unsigned int> > mPacketDefs; /// types(name) and total size
  std::vector<std::pair<std::vector<packType>, unsigned int> > m_packetDefs; /// types(name) and total size
};


#endif // PACKETDEF_H
