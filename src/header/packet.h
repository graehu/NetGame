#ifndef PACKET_H
#define PACKET_H

#include <cstring>

/// May want to write something to keep a track of what data is going into this.

class packet
{
	public:
	packet();
    ~packet();

	bool pushData(const unsigned char aData[], unsigned int aSize);
	bool clearPacket(void){mSize = 0; return false;}
	bool setAlloc(unsigned int aAlloc);
	unsigned int getSize(void){return mSize;}
	unsigned char* getData(void){return mData;}

	//stupid template functions can't be declared in one file and defined in another.
	template <typename T>
	bool pushData(T data)
	{
		unsigned int size = sizeof(data);
		//printf("data: %i\n", size);
		if((mSize + size) > mAlloc)
		{
			unsigned char* temp;
			temp = mData;
			mData = new unsigned char[(mSize + size)*2];
			memcpy(&mData[0], temp, mSize);
			delete temp;

			mAlloc = (mSize + size)*2;

		}

		for(unsigned int i = 0; i < size; i++)
		{
			if (i > 0)
				mData[mSize+i] = (data >> (size-(i+1))*8);
			else
				mData[mSize+i] = ((data >> (size-(i+1))*8) & 0xFF);
		}
		mSize += size;
		return false;
	};

    ///utility functions.  may be moved to private if i make a more general packing class.
    void writeInteger(unsigned char * data, unsigned int value);
    void readInteger(const unsigned char * data, unsigned int & value);
    void writeShort(unsigned char * data, unsigned short value);
    void readShort(const unsigned char * data, unsigned short & value);

	protected:
	private:

	unsigned char* mData; // the actual data
	unsigned int mSize;   // size current data takes up in the packet
	unsigned int mAlloc;  // amount of memory currently allocated to the packet

};
#endif//PACKET_H



