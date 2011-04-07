#ifndef PACKETDEF_H
#define PACKETDEF_H

#include <typeinfo>
#include <vector>
#include <cstdio>
#include <cassert>

class packetDef
{
	public:

    packetDef();
    ~packetDef();

    void beginPacket(void);
    template <typename T>
    bool pushType(T aType)
    {
        if(mPacking)
        {
            mPacketDefs.back().first.push_back((char*)typeid(aType).name());
            mPacketSizeAccumulator += getTypeSizeByName((char*)typeid(aType).name());
            return false;
        }
        else
        {
            printf("packetDef: pushType(), error need to beginPacket()\n");
            return true;
        }
    }
    unsigned int getPacketDefID(void){assert(mPacking);return mPacketDefs.size()-1;}
    void endPacket(void);


    char* getMembTypeName(unsigned int aDef, unsigned int aMember);
    unsigned int getTypeSizeByName(char* aName);
    unsigned int getDefSize(unsigned int aDefID);
    //unsigned int getNumOfMembsInDef(unsigned int aDef){if(aDef<mPacketDefs.size())return mPacketDefs[aDef].size();return 0;}
    unsigned int size(){return mPacketDefs.size();}

	protected:

	bool mPacking;

	private:

	unsigned int mPacketSizeAccumulator;

	std::vector<std::pair<std::vector<char*>, unsigned int> > mPacketDefs; /// types(name) and total size

};


#endif // PACKETDEF_H
