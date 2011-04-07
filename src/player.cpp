#include "header/player.h"

player::player() :
    entity()
{
    mName = {'I','\'','m',' ','c','o','n','s','t','r','c','t','e','d','\0'};
    mStrength = 0;
    mDefence = 0;
    mRange = 0;
    mHealth = 100;
}

void player::readInitPacket(void)
{


}

void player::readUpdatePacket(void)
{

}

void player::writeInitPacket(void)
{
    entity::writeInitPacket(); /// 18 bytes
    for(int i = 0; i < 16; i++)
        mPacket->pushData(mName[i]); /// 16 bytes
    mPacket->pushData(mStrength); /// 2
    mPacket->pushData(mDefence);  /// 2
    mPacket->pushData(mRange);    /// 2
    mPacket->pushData(mHealth);   /// 2

    /// 24 bytes + 18 bytes
    /// 42 bytes


}
void player::writeUpdatePacket(void)
{
    entity::writeUpdatePacket();

}
