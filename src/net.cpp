#include "header/net.h"
#include <cstdlib>

using namespace net;

network::network(unsigned short _protocolID, float _timeout , unsigned int _maxSequence) :
  connection(_protocolID, _timeout, _maxSequence)
{
  m_packetSize = 256; ///bad, should be passed in constructor or something.
}

network::~network()
{
}

///this type bull isn't a good solution.
bool network::init(bool _host, int _port)
{

  if(!start(_port))
    {
      printf("could not start connection on port %d\n", _port);
      return true;
    }
  m_host = _host;
      
  /*if(_host == false)
    {
      m_host = false;
      //TODO add entity to entityHandler is not host.
      /*netEntity* me = new netEntity;
      m_entities.push_back(me);

	
      address myAddress(127,0,0,1,8000); //this shit should be passed.
      connect(myAddress);
      m_enUpdate.push_back(vector<enInfo>());
      m_enUpdate[0].push_back(enInfo(0,e_updating));*//*
      ///telling the server about myself
    }

  if(_host == true)
    {
      m_host = true;
      //entity* me = new entity;
      //mEntities.push_back(me);
    }*/


      
  return false;
}
	  
bool network::update(float _deltaTime)
{
  connection::update(_deltaTime);

  while(receivePacket(m_packetSize) != 0)
  {
	 m_receivePacket.setEnd(m_packetSize);
	 m_handlers[m_receivePacket.readHandleID()]->readData((packet*)&m_receivePacket);
  }
  ///The number of people that need to be sent to.
  for(unsigned int i = 0; i < getMailListSize(); i++)
  {
	  m_handlers[0]->writeData((packet*)&m_receivePacket);
  }

	/*
  while(receivePacket(m_packetSize) != 0)
    {
      //printf("oh dear god, the horror\n");
      ///first the connection checks if it has any packets to read


      unsigned short packetSender = m_receivePacket.readKey();
      unsigned short offset = m_receivePacket.getHeaderSize();
      unsigned short enType = 0;
      unsigned short accessKey = 0;
      unsigned char commands = 0;
      unsigned int j = 0;
      int xvelocity = 0;
      int yvelocity = 0;
      int xpos = 0;
      int ypos = 0;
      bool data = true;

      /// //////////////////////
      /// Packet Read Section///
      /// //////////////////////

    while(data)
	{
	  //printf("curious\n");
	  switch(m_receivePacket.getSectionType(j))
	    {
	    case 0:
	      ///empty
	      data = false;
	      break;
	    case 1:
	      /// update
	      //m_receivePacket.getMembData(j,1,enType);

	      m_receivePacket.getMembData(j,1,accessKey);

	      initEntity(packetSender, accessKey);

	      if (m_host == false)
		{
		  xpos = m_receivePacket.readInteger(offset+4);
		  ypos = m_receivePacket.readInteger(offset+8);
		  xvelocity = m_receivePacket.readInteger(offset+12);
		  yvelocity = m_receivePacket.readInteger(offset+16);
		  m_entities[m_enUpdate[packetSender][accessKey].m_enKey]->setXPos(xpos);
		  m_entities[m_enUpdate[packetSender][accessKey].m_enKey]->setYPos(ypos);
		  //m_entities[m_enUpdate[packetSender][accessKey].m_enKey]->setXVelocity(xvelocity);
		  //m_entities[m_enUpdate[packetSender][accessKey].m_enKey]->setYVelocity(yvelocity);
		}
	      //else{printf("do i want to see this... hmm\n");}
	      break;
	    case 2:
	      ///update
	      m_receivePacket.getMembData(j,1,accessKey);

	      initEntity(packetSender, accessKey);

	      if (m_host == true)
		{
		  m_receivePacket.getMembData(j, 2, commands);
		  m_entities[m_enUpdate[packetSender][accessKey].m_enKey]->setCommands(commands);
		  //mEntities[mEnUpdate[packetSender][accessKey].mEnKey]->move();

		}
	      break;
	    }
	  j++;
	  offset = m_receivePacket.getSectionStart(j);

	}
    }
  /// ///////////////////////
  /// Packet Write Section///
  /// ///////////////////////

  for(int i = 0; i < m_enUpdate.size(); i++)
    {
      for(int ii = 0; ii < m_enUpdate[i].size(); ii++)
	{
	  /// check if entity is initialised, if so, send update
	  /// packet
	  if(m_enUpdate[i][ii].m_state == e_updating)
	    {
	      if(m_host == true)
		{
		  m_sendPacket.beginPacketData(1);
		  m_sendPacket.safePushData((unsigned short)ii);
		  m_sendPacket.safePushData((int)m_entities[m_enUpdate[i][ii].m_enKey]->getXPos());
		  m_sendPacket.safePushData((int)m_entities[m_enUpdate[i][ii].m_enKey]->getYPos());
		  m_sendPacket.safePushData((unsigned char)m_entities[m_enUpdate[i][ii].m_enKey]->getCommands());
		  m_sendPacket.endPacketData();

		}

	      if(m_host == false && ii == 0)
		{
		  m_sendPacket.beginPacketData(2);
		  m_sendPacket.safePushData((unsigned short)ii);
		  m_sendPacket.safePushData((unsigned char)m_entities[m_enUpdate[i][ii].m_enKey]->getCommands());
		  m_sendPacket.endPacketData();
		}
	    }

	  // if(mEnUpdate[i][ii].mState == eUninitialised)
	}
      /// need REAL delta time for packet...
      //if(mHost == true)
      //{
      //    printf("Sending\n");
      //}
      m_sendPacket.pushData((unsigned short)0);
      sendPacket(i, _deltaTime);
      m_sendPacket.clearPacket();
    }


  for(int i = 0; i < m_entities.size(); i++)
    m_entities[i]->move();*/
		
  return false;
		  
}
/*
void network::initEntity(unsigned short _packetSender, unsigned short _accessKey)
{
  /// /////////////////////////////////////////////////////////
  /// make sure that there is an update slot for this sender///
  /// and push back                                         ///
  /// /////////////////////////////////////////////////////////
  while(_packetSender >= m_enUpdate.size()) /// if i don't have a place for this sender, make one.
    {
      m_enUpdate.push_back(vector<enInfo>());
    }
  /// ////////////////////////////////////////////////////
  /// now i want to check if i have this entity or not.///
  /// ////////////////////////////////////////////////////

  if(_accessKey == m_enUpdate[_packetSender].size())
    {

      /// the important thing to realise is that you are trying to keep each packet senders
      /// list the same on BOTH sides, so, the servers access key will be the same as the clients
      /// the only thing that should change is the entity id.

      /// i want him to know about all entities, for now so see how many entities there are
      /// and push that many onto his unknown list.


      netEntity* nEntity = new netEntity;
      m_entities.push_back(nEntity);
      if(m_port == 8005)printf("8005 initialising entity\n");
      m_enUpdate[_packetSender].push_back(enInfo((m_entities.size()-1),e_updating));
      //if(mHost == true)

      /*for(int i = 0; i < mEntities.size(); i++)
	{
	if(i >= mEnUpdate[packetSender].size())
	{
	mEnUpdate[packetSender].push_back(enInfo(i,eUpdating));
	printf("pushing back entity to be initialised\n");
	}

	}*//*

      for(unsigned int i = 0; i < m_entities.size(); i++)
	{
	  if(i >= m_enUpdate[_packetSender].size())
	    {
	      /// this is the fix for the first client bug.
	      /// basically, because the main character is always
	      /// pushed back as zero it means that the ACTUAL entity zero
	      /// will never get into this if. (because it only gets in if
	      /// i >= number of updateEns, which there is always one of
	      /// by this point. so zero wont get in, because it's less than one)
	      /// the reason for the following behaviour was because the same enkey
	      /// was being pushed back twice. the packetsender's en key.
	      /// so, to fix this, push back 0 if you come across the packetsender's
	      /// enKey.
	      if(i == m_entities.size()-1)
		m_enUpdate[_packetSender].push_back(enInfo(0,e_updating));
	      else
		m_enUpdate[_packetSender].push_back(enInfo(i,e_updating));
	      printf("pushing back Entity to be initialised\n");
	      //printf("");

	    }

	}

      /// this part should change if you don't want to tell everybody about this new entity
      /// but i do.

      /// ok so, someone new comes in, number one
      /// all people from now on will be told about number one.

      for(int i = 0; i < m_enUpdate.size(); i++)
	{
	  if(i != _packetSender)
	    {
	      m_enUpdate[i].push_back(enInfo((m_entities.size()-1), e_updating));
	    }
	}

      /// and now tell the sender about everyone else.

    }
}
*/

void network::registerHandler(netHandler* _handler)
{
	m_handlers.push_back(_handler);
}
