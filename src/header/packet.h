#ifndef PACKET_H
#define PACKET_H

#include <cstring>
#include <cstdio>
#include <vector>


#include "dataUtils.h"


namespace net
{

class packet
{
	public:
	packet();
    ~packet();

    /// Packet data related functions

	bool clearPacket(void){m_end = m_headerSize; return false;}
	bool setAlloc(unsigned int _alloc);
	void setEnd(unsigned int _end){if(_end >! m_alloc && _end > m_headerSize)m_end = _end;}

	unsigned int getAlloc(void) {return m_alloc;}
	unsigned int getEnd(void){return m_end;}
	unsigned char* getData(void){return m_data;}
	unsigned int getHeaderSize(void){return m_headerSize;}

	template <typename T>
	void iterWrite(T _type)
	{
		dataUtils::instance().writeData(_type, &m_data[m_end]);
		m_end += sizeof(T);
	}
	template <typename T>
	T iterRead(void)
	{
		T temp =  dataUtils::instance().readData<T>(&m_data[m_end]);
		m_end += sizeof(T);
		return temp;
	}

	protected:


	unsigned int m_alloc;  // amount of memory currently allocated to the packet
	unsigned char* m_data; // the actual data
	unsigned int m_end;   // the End offset, how far it is to the end of the current data-set
	bool m_packing;



	private:

	unsigned int m_sizeAccumulator;
	unsigned int m_headerSize;   /// = 16;




};
}
#endif//PACKET_H














////Code graveyard

//public:
/// header realated functionality.



	/// stupid template functions can't be declared in one file and defined in another.


  /// header related functionality.
  //void analysePacket(void);
  //unsigned int getSectionType(unsigned int _section);
  //unsigned int getSectionStart(unsigned int _section);
  //unsigned char* getData(void){return m_data;}

  /*bool beginPacketData(unsigned int _packetDefID);
	template <typename T>
	bool safePushData(T _data)
	{
	    if(m_packing)
	    {
	    	char* name = defRegister::instance().getDef(m_currentDefID).getMembTypeName(m_typeIttorator);
          if(typeid(T).name() == name)
          {
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
              //printf("i'm pushing, right?\n");
              writeData(_data,m_end);

              m_sizeAccumulator += size;
              m_end += size;
              m_typeIttorator++;
              return false;
          }
          else
          {
              printf("packet: safePushData(), error expected:%s passed:%s\n",
                     (const char*)name,
                     typeid(T).name());
              return true;
          }
		}
		else
		{
		    printf("packet: safePushDate(), error haven't begun packet\n");
          return true;
		}
	};
  bool endPacketData(void);*/

  /*template <typename T>
  void getMembData(unsigned int _section, unsigned int _member, T &_data)
  {
      if(_section < m_sectionInfo.size())
      {
          if(_member < m_sectionInfo[_section].size())
          {
              unsigned int offset = m_sectionInfo[_section][_member].second;
              if(typeid(_data).name() != m_sectionInfo[_section][_member].first)
              	printf("packet: getMemData(), warning passing incorrect type to be filled.\n");
              readDataIntoType(offset, _data);
          }
      }
  }*/

  /*template <typename T>
  void readDataIntoType(unsigned short _offset, T &_data)
  {
      if(typeid(_data) == typeid(short))
      {
          _data = readShort(_offset);
          return;
      }
      if(typeid(_data) == typeid(unsigned short))
      {
          _data = readUShort(_offset);
          return;
      }
      if(typeid(_data) == typeid(int))
      {
          _data = readInteger(_offset);
          return;
      }
      if(typeid(_data) == typeid(unsigned int))
      {
          _data = readUInteger(_offset);
          return;
      }
      if(typeid(_data) == typeid(char))
      {
          _data = readChar(_offset);
          return;
      }
      if(typeid(_data) == typeid(unsigned char))
      {
          _data = readUChar(_offset);
          return;
      }

      printf("failure on type recognition\n");
  }*/

//private:

//TODO turn this into a definition pointer?
//TODO at least make this use packType instead of char*
//std::vector<std::vector<std::pair<char*, unsigned int> > > m_sectionInfo; ///vector<type(name)> and offset(start of section).
//std::vector<unsigned int> m_sectionIDs;
//std::vector<std::vector<std::pair<packType, unsigned int> > > m_sectionInfo;
//unsigned int m_currentDefID;
//unsigned int m_typeIttorator;


