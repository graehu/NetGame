#include "header/net.h"
#include <cstdlib>

using namespace net;

network::network(unsigned short _protocolID, float _timeout , unsigned int _maxSequence) :
  connection(_protocolID, _timeout, _maxSequence)
{
  m_packetSize = 256;
}

network::~network()
{
}

///TODO this type boolean isn't a good solution.
bool network::init(bool _host, int _port)
{

  if(!start(_port))
    {
      printf("could not start connection on port %d\n", _port);
      return 1;
    }
      
  if(_host == false)
    {
      m_host = false;
      netEntity* me = new netEntity;
      m_entities.push_back(me);
	
      address myAddress(127,0,0,1,8000); //this shit should be passed.
      connect(myAddress);
      m_enUpdate.push_back(vector<enInfo>());
      m_enUpdate[0].push_back(enInfo(0,e_updating));
      ///telling the server about myself
    }

  if(_host == true)
      m_host = true;

      
  return false;
}
	  
bool network::update(float _deltaTime)
{
  connection::update(_deltaTime);

  /// ///////////////////////
  /// Packet read Section////
  /// ///////////////////////

  while(receivePacket(m_packetSize) != 0)
  {
	unsigned short packetSender = m_receivePacket.readKey();
	unsigned short accessKey = 0;
	unsigned short entityKey = 0;

	while(m_receivePacket.getEnd() < getBytesRead())
	{
		packets sectionType = (packets)m_receivePacket.iterRead<unsigned short>();
		switch(sectionType)
		{
		case e_initPacket:
			break;
		case e_playerUpdatePacket:

			accessKey = m_receivePacket.iterRead<unsigned short>();
			initEntity(packetSender, accessKey);
			if (m_host == true)
			{
				entityKey = m_enUpdate[packetSender][accessKey].m_enKey;
				m_entities[entityKey]->setCommands(m_receivePacket.iterRead<unsigned char>());
			}

			break;
		case e_entityUpdatePacket:
			while(m_receivePacket.getEnd() < getBytesRead())
				{
					accessKey = m_receivePacket.iterRead<unsigned short>();
				    initEntity(packetSender, accessKey);

				    entityKey = m_enUpdate[packetSender][accessKey].m_enKey; //entity key
				    if(m_host == false)
				    	m_entities[entityKey]->readPacket(&m_receivePacket);
				}
			break;
		}
	}
  }
  /// ///////////////////////
  /// Packet Write Section///
  /// ///////////////////////
  {
  unsigned short entityKey = 0;
  for(unsigned short i = 0; i < m_enUpdate.size(); i++)
    {
	  if(m_host)
    	  m_sendPacket.iterWrite((unsigned short)e_entityUpdatePacket);

      for(unsigned short ii = 0; ii < m_enUpdate[i].size(); ii++)
	{
	  if(m_enUpdate[i][ii].m_state == e_updating)
	    {

	      if(m_host == true)
		{
	      entityKey = m_enUpdate[i][ii].m_enKey;
	      m_sendPacket.iterWrite(ii);
	      m_entities[entityKey]->writePacket(&m_sendPacket);
		}

	      if(m_host == false && ii == 0)
		{

	    	  entityKey = m_enUpdate[i][ii].m_enKey;
	    	  m_sendPacket.iterWrite((unsigned short)e_playerUpdatePacket);
	    	  m_sendPacket.iterWrite(ii);
	    	  m_sendPacket.iterWrite(m_entities[entityKey]->getCommands());
		}
	    }

	}

      sendPacket(i, _deltaTime);
      m_sendPacket.clearPacket();
    }

  for(int i = 0; i < m_entities.size(); i++)
    m_entities[i]->move();

  }
		
  return false;
		  
}

void network::initEntity(unsigned short _packetSender, unsigned short _accessKey)
{

  while(_packetSender >= m_enUpdate.size())
    {
      m_enUpdate.push_back(vector<enInfo>());
    }

   if(_accessKey == m_enUpdate[_packetSender].size())
    {

      netEntity* nEntity = new netEntity;
      m_entities.push_back(nEntity);
      if(m_host)printf("initialising entity\n");
      m_enUpdate[_packetSender].push_back(enInfo((m_entities.size()-1),e_updating));



      for(unsigned int i = 0; i < m_entities.size(); i++)
	{
	  if(i >= m_enUpdate[_packetSender].size())
	    {
	      if(i == m_entities.size()-1)
		m_enUpdate[_packetSender].push_back(enInfo(0,e_updating));
	      else
		m_enUpdate[_packetSender].push_back(enInfo(i,e_updating));
	      printf("pushing back Entity to be initialised\n");
	    }
	}

      for(int i = 0; i < m_enUpdate.size(); i++)
	{
	  if(i != _packetSender)
	    {
	      m_enUpdate[i].push_back(enInfo((m_entities.size()-1), e_updating));
	    }
	}


    }
}

void network::draw()
{
  m_renderer.beginScene();
  for(int i = 0; i < m_entities.size(); i++)
    {
      m_renderer.draw(m_entities[i]->getXPos(),
		    m_entities[i]->getYPos(),
		    1,
		    1);
    }
  m_renderer.endScene();
}

netEntity* network::getEntity(unsigned int _element)
{
  if(_element < m_entities.size())
    return m_entities[_element];

  return 0;
}

void network::addEntity(void)
{

  netEntity* nEntity = new netEntity;
  m_entities.push_back(nEntity);

  m_enUpdate[0].push_back(enInfo((m_entities.size()-1),e_updating));

  for(int i = 0; i < m_entities.size(); i++)
    {
      m_enUpdate[i].push_back(enInfo((m_entities.size()-1),e_uninitialised));
    }

  printf("%i new entity added!\n", m_port);
}

