#include "ObjectTimer.h"

CObjectTimer::CObjectTimer(void)
{
}

CObjectTimer::~CObjectTimer(void)
{
}
CObjectTimer::CObjectTimer(long i): m_TimeInterval(i), m_lastTime(0){}
void CObjectTimer::SetTime(long i){ m_TimeInterval = i; }
BOOL CObjectTimer::Run(){
	if(GetTickCount() - m_lastTime < m_TimeInterval)
		return FALSE;
	m_lastTime = GetTickCount();
	return TRUE;
}
BOOL CObjectTimer::RunOnce(){

	return TRUE;
}
