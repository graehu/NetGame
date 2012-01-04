#ifndef ADDRESS_H
#define ADDRESS_H
namespace net
{

  class address
  {
  public:

    address();
    address(unsigned char _a, unsigned char _b, unsigned char _c, unsigned char _d, unsigned short _port);
    address(unsigned int _address, unsigned short _port);

    unsigned int getAddress() const;
    unsigned char getA() const;
    unsigned char getB() const;
    unsigned char getC() const;
    unsigned char getD() const;
    unsigned short getPort() const;

    bool operator == (const address & other) const;
    bool operator != (const address & other) const;
    bool operator < (const address & other) const;

  private:
    unsigned int m_address;
    unsigned short m_port;
  };

}
#endif//ADDRESS_H
