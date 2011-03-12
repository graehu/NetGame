#ifndef FLOWCONTROL_H
#define FLOWCONTROL_H

#include <stdio.h>

namespace net
{

class FlowControl
{
public:
	
	FlowControl();
	
	void Reset();
	void Update( float deltaTime, float rtt );
	float GetSendRate();
	
private:

	enum Mode
	{
		Good,
		Bad
	};

	Mode mode;
	float penalty_time;
	float good_conditions_time;
	float penalty_reduction_accumulator;
};

}

#endif//FLOWCONTROL_H

