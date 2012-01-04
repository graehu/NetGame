#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include <stdio.h>

namespace net
{

class flowControl
{
public:
	
	flowControl();
	
	void reset();
	void update(float _deltaTime, float _rtt);
	float getSendRate();
	
private:

	enum mode
	{
		e_good = 0,
		e_bad
	};

	mode m_mode;
	float m_penaltyTime;
	float m_goodConditionsTime;
	float m_penaltyReductionAccumulator;
};

}

#endif//FLOWCONTROL_H

