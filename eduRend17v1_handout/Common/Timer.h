#pragma once


class Timer
{
public:
	static Timer Now();

	Timer operator-(Timer& RHS);
	float Seconds();
private:
	static __int64 COUNTS_PER_SECOND;
	static const float SECONDS_PER_COUNT;
	__int64 m_Counts;

	Timer(__int64 Counts);
};

