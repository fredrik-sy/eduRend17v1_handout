#include "stdafx.h"
#include "Timer.h"

__int64 Timer::COUNTS_PER_SECOND;
const float Timer::SECONDS_PER_COUNT = QueryPerformanceFrequency((LARGE_INTEGER*)&Timer::COUNTS_PER_SECOND) ? 1.0f / Timer::COUNTS_PER_SECOND : 0.0f;

Timer Timer::Now()
{
	__int64 Counts;
	QueryPerformanceCounter((LARGE_INTEGER*)&Counts);
	return Timer(Counts);
}


Timer Timer::operator-(Timer& RHS)
{
	return Timer(this->m_Counts - RHS.m_Counts);
}


float Timer::Seconds()
{
	return m_Counts * Timer::SECONDS_PER_COUNT;
}


Timer::Timer(__int64 Counts)
{
	m_Counts = Counts;
}
