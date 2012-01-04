#include "header/address.h" 
using namespace net;

address::address()
{
  m_address = 0;
  m_port = 0;
}

address::address(unsigned char _a, unsigned char _b, unsigned char _c, unsigned char _d, unsigned short _port)
{
  this->m_address = (_a << 24) | (_b << 16) | (_c << 8) | _d;
  this->m_port = _port;
}

address::address( unsigned int _address, unsigned short _port )
{
  this->m_address = _address;
  this->m_port = _port;
}

unsigned int address::getAddress() const
{
  return m_address;
}

unsigned char address::getA() const
{
  return (unsigned char) (m_address >> 24);
}

unsigned char address::getB() const
{
  return (unsigned char) (m_address >> 16);
}

unsigned char address::getC() const
{
  return (unsigned char) (m_address >> 8);
}

unsigned char address::getD() const
{
  return (unsigned char) (m_address);
}

unsigned short address::getPort() const
{ 
  return m_port;
}

bool address::operator == (const address & other) const
{
  return m_address == other.m_address && m_port == other.m_port;
}

bool address::operator != (const address & other) const
{
  return ! (*this == other);
}
	
bool address::operator < (const address & other) const
{
  // note: this is so we can use address as a key in std::map
  // note: I wont be using this :)
  if (m_address < other.m_address)
    return true;
  if (m_address > other.m_address)
    return false;
  else
    return m_port < other.m_port;
}
