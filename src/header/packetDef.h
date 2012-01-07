#ifndef PACKETDEF_H
#define PACKETDEF_H

#include <vector>
#include <typeinfo>
#include <cstdio>
#include <cassert>

namespace net
{

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

class packetDef;
class defRegister
{
public:

	//TODO think of a way to hide this functionality.
	unsigned int registerDefinition(packetDef* _definition)
	{
		m_definitions.push_back(_definition);
		return m_definitions.size()-1;
	}
	packetDef& getDef(unsigned int _defID);
	unsigned int getSize(void){return m_definitions.size();}

  char* getNameByType(packType _type);
  packType getTypeByName(char* _name);
  unsigned int getTypeSizeByName(char* _name);
  unsigned int getTypeSizeByPackType(packType _type);

  //singleton
	static defRegister& instance()
	{
		static defRegister* instance;
		if(!instance)
			instance = new defRegister();
		return *instance;
	}

protected:
private:
	defRegister();
	~defRegister();
	std::vector<packetDef*> m_definitions;

};

/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

class packetDef
{
 public:

  packetDef();
  ~packetDef();

  void beginDefinition(void);
  template <typename T>
    bool pushType(T _type)
    {
      if(m_packing)
	  {
		  packType myType;
		  myType = defRegister::instance().getTypeByName((char*)typeid(_type).name());
		  m_defintion.push_back(myType);
		  m_size += defRegister::instance().getTypeSizeByPackType(myType);
		  return false;
	  }
      else
	  {
	  printf("packetDef: pushType(), error need to beginPacket()\n");
	  return true;
	  }
    }

  void endDefinition(void);

  char* getMembTypeName(unsigned int _member);

  unsigned int getID(void){return m_ID;}
  unsigned int getSize(){return m_size;}

 protected:

  bool m_packing;

 private:

  unsigned int m_size;
  unsigned int m_ID;

  std::vector<packType> m_defintion;
};
}

#endif // PACKETDEF_H
