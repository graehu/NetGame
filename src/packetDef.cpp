#include "header/packetDef.h"
#include <cstdio>

packetDef::packetDef()
{
    mPacking = false;
    ///first define == empty packet.
    mPacketDefs.push_back(std::pair<std::vector<char*>, unsigned int>());
    mPacketDefs[0].first.push_back(0);
    mPacketDefs[0].second = 0;
    mPacketSizeAccumulator = 0;
}

packetDef::~packetDef()
{

}

void packetDef::beginPacket(void)
{
    if(!mPacking)
    {
        mPacking = true;
        mPacketDefs.push_back(std::pair<std::vector<char*>, unsigned int>());
        mPacketDefs.back().first.push_back((char*)typeid(unsigned short).name());
        mPacketSizeAccumulator += sizeof(unsigned short);
        return;
    }
    if(mPacking){printf("packetDef: beginPacket(), error packet already begun\n"); return;}
};


void packetDef::endPacket(void)
{
    if(!mPacking){printf("packetDef: endPacket(), error haven't begun, so can't end\n"); return;}
    if(mPacking)
    {
        if(mPacketDefs.back().first.size() == 1 && mPacketDefs.size() > 0)
        {
            mPacketDefs.pop_back();
            mPacketSizeAccumulator = 0;
            mPacking = false;
            return;
        }
        else
        {
            mPacketDefs.back().second = mPacketSizeAccumulator;
            mPacketSizeAccumulator = 0;
            mPacking = false;
            return;
        }
    }
}

char* packetDef::getMembTypeName(unsigned int aDef, unsigned int aMember)
{

    if(aDef < mPacketDefs.size())
        if(aMember < mPacketDefs[aDef].first.size())
            return mPacketDefs[aDef].first[aMember];

    //printf("packetDef: getMemberType(), error invalid arguments, out of bounds\n");
    return 0;
} /// which definition, then which member.

unsigned int packetDef::getDefSize(unsigned int aDefID)
{
    if(aDefID < mPacketDefs.size())
        return mPacketDefs[aDefID].second;
    printf("packetDef: getDefSize(), error aDefID too high\n");
    return 0;
}

unsigned int packetDef::getTypeSizeByName(char* aName)
{
    ///find out how to replace this with a switch block
    if(aName == typeid(bool).name())
        return sizeof(bool);
    if(aName == typeid(unsigned char).name())
        return sizeof(unsigned char);
    if(aName == typeid(char).name())
        return sizeof(char);
    if(aName == typeid(unsigned short).name())
        return sizeof(unsigned short);
    if(aName == typeid(short).name())
        return sizeof(short);
    if(aName == typeid(unsigned int).name())
        return sizeof(unsigned int);
    if(aName == typeid(int).name())
        return sizeof(int);
    if(aName == typeid(float).name())
        return sizeof(float);
    if(aName == typeid(double).name())
        return sizeof(double);
    printf("not knowing this type\n");
    assert(false);
}
