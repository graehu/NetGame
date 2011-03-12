#include "header/address.h" 
using namespace net;

address::address()
{
	mAddress = 0;
	mPort = 0;
}

address::address( unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short nPort )
{
	this->mAddress = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
	this->mPort = nPort;
}

address::address( unsigned int nAddress, unsigned short nPort )
{
	this->mAddress = nAddress;
	this->mPort = nPort;
}

unsigned int address::getAddress() const
{
	return mAddress;
}

unsigned char address::getA() const
{
	return ( unsigned char ) ( mAddress >> 24 );
}

unsigned char address::getB() const
{
	return ( unsigned char ) ( mAddress >> 16 );
}

unsigned char address::getC() const
{
	return ( unsigned char ) ( mAddress >> 8 );
}

unsigned char address::getD() const
{
	return ( unsigned char ) ( mAddress );
}

unsigned short address::getPort() const
{ 
	return mPort;
}

bool address::operator == ( const address & other ) const
{
	return mAddress == other.mAddress && mPort == other.mPort;
}

bool address::operator != ( const address & other ) const
{
	return ! ( *this == other );
}
	
bool address::operator < ( const address & other ) const
{
	// note: this is so we can use address as a key in std::map
	if ( mAddress < other.mAddress )
		return true;
	if ( mAddress > other.mAddress )
		return false;
	else
		return mPort < other.mPort;
}
