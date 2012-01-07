#include "header/entityHandler.h"
#include "header/dataUtils.h"
using namespace net;
using namespace std;


void entityHandler::notify(events _event)
{

}

void entityHandler::readData(packet* _packet)
{
	//This is the data stripped of the header
	unsigned char* p_data = &_packet->getData()[_packet->getHeaderSize()];
	//unsigned short l_packetSender = _packet->readKey();// THIS WONT BE NEEDED EVENTUALLY
	unsigned short l_packetSender = dataUtils::instance().readUShort(&_packet->getData()[2]);
	unsigned short l_accessKey = dataUtils::instance().readUShort(&p_data[0]);
	unsigned short l_offset = 2;

	initEntity(l_packetSender, l_accessKey);
	while(l_offset < _packet->getEnd())
	{
		unsigned short l_entityID = m_enUpdate[l_packetSender][l_accessKey].m_enKey;
		m_entities[l_entityID]->readPacket(&p_data[l_offset]);
		l_offset += m_entities[l_entityID]->getDefSize();
		l_accessKey = dataUtils::instance().readUShort(&p_data[l_offset]);
		initEntity(l_packetSender, l_accessKey);
	}
}

void entityHandler::writeData(packet* _packet)
{
	unsigned char* p_data = &_packet->getData()[_packet->getHeaderSize()];
	unsigned short l_offset = 0;

	for(unsigned int i = 0; i < m_entities.size(); i++)
	{
		dataUtils::instance().writeData((unsigned short)i, &p_data[l_offset]);
		l_offset += 2;
		m_entities[i]->writePacket(&p_data[l_offset]);
		l_offset += m_entities[i]->getDefSize();
	}
}


void entityHandler::draw(void)
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


netEntity* entityHandler::getEntity(unsigned int _element)
{
  if(_element < m_entities.size())
    return m_entities[_element];

  return 0;
}



// THIS IS DUMB AS SHIT! FUCKING ARGH!

void entityHandler::initEntity(unsigned short _packetSender, unsigned short _accessKey)
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
      //if(m_port == 8005)printf("8005 initialising entity\n");
      m_enUpdate[_packetSender].push_back(enInfo((m_entities.size()-1),e_updating));
      //if(mHost == true)

      /*for(int i = 0; i < mEntities.size(); i++)
	{
	if(i >= mEnUpdate[packetSender].size())
	{
	mEnUpdate[packetSender].push_back(enInfo(i,eUpdating));
	printf("pushing back entity to be initialised\n");
	}

	}*/

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
//*/
