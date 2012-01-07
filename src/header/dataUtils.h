#ifndef PACKETUTILS_H
#define PACKETUTILS_H


class dataUtils
{
public:

	static dataUtils& instance(void)
	{
		dataUtils* instance;
		if(!instance)
			instance = new dataUtils();
		return *instance;
	}

    int readInteger(unsigned char* _data);
    unsigned int readUInteger(unsigned char* _data); ///these return the data at the offset, with the type used.

    short readShort(unsigned char* _data);
    unsigned short readUShort(unsigned char* _data);

    char readChar(unsigned char* _data);
    unsigned char readUChar(unsigned char* _data);


	template <typename T>
    bool writeData(T _type, unsigned char* _data)
	{
		unsigned int size = sizeof(_type);
		for(unsigned int i = 0; i < size; i++)
		{
			if (i > 0) _data[i] = (_type >> (size-(i+1))*8);
			else       _data[i] = ((_type >> (size-(i+1))*8) & 0xFF);
		}
		return false;
	}

private:
	dataUtils(){};
	~dataUtils(){};

};

#endif//PACKETUTILS_H
