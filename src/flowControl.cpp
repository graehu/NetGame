#include "header/flowControl.h"
using namespace net;


// this flowcontrol only uses rtt as a means to work out how fast the flow should be.
// look into making this better.
// should probably take packet loss into account too... no?



flowControl::flowControl()
{
	//printf( "flow control initialized\n" );
	reset();
}

void flowControl::reset()
{
	m_mode = e_bad;
	m_penaltyTime = 4.0f;
	m_goodConditionsTime = 0.0f;
	m_penaltyReductionAccumulator = 0.0f;
}

void flowControl::update(float _deltaTime, float _rtt)
{
	const float RTT_Threshold = 250.0f;

	if (m_mode == e_good)
	{
		if (_rtt > RTT_Threshold)
		{
			///printf( "*** dropping to bad mode ***\n" );
			m_mode = e_bad;
			if (m_goodConditionsTime < 10.0f && m_penaltyTime < 60.0f)
			{
				m_penaltyTime *= 2.0f;
				if (m_penaltyTime > 60.0f)
					m_penaltyTime = 60.0f;
				///printf("penalty time increased to %.1f\n", penalty_time);
			}
			m_goodConditionsTime = 0.0f;
			m_penaltyReductionAccumulator = 0.0f;
			return;
		}

		m_goodConditionsTime += _deltaTime;
		m_penaltyReductionAccumulator += _deltaTime;

		if (m_penaltyReductionAccumulator > 10.0f && m_penaltyTime > 1.0f)
		{
			m_penaltyTime /= 2.0f;
			if (m_penaltyTime < 1.0f)
				m_penaltyTime = 1.0f;
			///printf("penalty time reduced to %.1f\n", penalty_time);
			m_penaltyReductionAccumulator = 0.0f;
		}
	}

	if (m_mode == e_bad)
	{
		if (_rtt <= RTT_Threshold)
			m_goodConditionsTime += _deltaTime;
		else
			m_goodConditionsTime = 0.0f;

		if (m_goodConditionsTime > m_penaltyTime)
		{
			///printf("*** upgrading to good mode ***\n");
			m_goodConditionsTime = 0.0f;
			m_penaltyReductionAccumulator = 0.0f;
			m_mode = e_good;
			return;
		}
	}
}

float flowControl::getSendRate()
{
	return m_mode == e_good ? 30.0f : 10.0f;
}
