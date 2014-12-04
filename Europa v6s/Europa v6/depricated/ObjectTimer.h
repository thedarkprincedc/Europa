#pragma once
#include "../PHNX ENGINE/phoenixengine.h"
class CObjectTimer
{
	long m_lastTime;
	long m_TimeInterval;	// Timing Settings
public:
	CObjectTimer(void);
	CObjectTimer(long i);
	void SetTime(long i);
	BOOL Run();
	BOOL RunOnce();

public:
	~CObjectTimer(void);
};
