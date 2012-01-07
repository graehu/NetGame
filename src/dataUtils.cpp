#include "header/dataUtils.h"


unsigned int dataUtils::readUInteger(unsigned char* _data)
{
    unsigned int number = (((unsigned int)_data[0] << 24) | ((unsigned int)_data[1] << 16) |
                           ((unsigned int)_data[2] << 8)  | ((unsigned int)_data[3]));
    return number;
}

int dataUtils::readInteger(unsigned char* _data)
{
    int number = (((int)_data[0] << 24) | ((int)_data[1] << 16) |
                  ((int)_data[2] << 8)  | ((int)_data[3]));
    return number;
}

unsigned short dataUtils::readUShort(unsigned char* _data)
{
    unsigned short number = (((unsigned short)_data[0] << 8) |
                    ((unsigned short)_data[1]));

    return number;
}

short dataUtils::readShort(unsigned char* _data)
{
    short number = (((short)_data[0] << 8) |
                    ((short)_data[1]));
    return number;
}


unsigned char dataUtils::readUChar(unsigned char* _data)
{


    unsigned char character = ((unsigned char)*_data);

    return character;
}

char dataUtils::readChar(unsigned char* _data)
{
    char character = ((char)*_data);

    return character;
}
