#ifndef ADDRESS_H
#define ADDRESS_H
namespace net
{

class address
{
public:

	address();
	address( unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port );
	address( unsigned int address, unsigned short port );

	unsigned int getAddress() const;
	unsigned char getA() const;
	unsigned char getB() const;
	unsigned char getC() const;
	unsigned char getD() const;
	unsigned short getPort() const;

	bool operator == ( const address & other ) const;
	bool operator != ( const address & other ) const;
	bool operator < ( const address & other ) const;

private:
	unsigned int mAddress;
	unsigned short mPort;
};

}
#endif//ADDRESS_H
