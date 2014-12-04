/******************************************
	Core System - Takes Care of Win32 
				  Initalization
	Note: You only need to derive your game
		  Model/View class from CApplication
    Classes: CApplication -> Done
			 CStateManager -> Needs Work(although used)
			 CFrameRate -> Need to check
*******************************************/
#ifndef _CORE_SYSTEM_H_
#define _CORE_SYSTEM_H_

class CApplication{
private:
	HINSTANCE	m_hInst;				//	Handle to Instance
	HWND		m_hWnd;					//	Handle to hwnd
protected:
	char		m_Class[255];		
	char		m_Caption[255];
	WNDCLASSEX	m_wcex;
	DWORD		m_Style;
	DWORD		m_XPos;
	DWORD		m_YPos;
	DWORD		m_Width;
	DWORD		m_Height;
public:
	CApplication();
	HWND		GethWnd();
	HINSTANCE	GethInst(); 

	BOOL Run();
	BOOL Error(BOOL fatal, char *text);
	BOOL Move(long XPos, long YPos);
	BOOL Resize(long width, long height);
	BOOL ShowMouse(BOOL Show = TRUE);
	virtual int FAR PASCAL MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{	return (int)DefWindowProc(hWnd, uMsg, wParam, lParam);	}
	virtual BOOL Init()		{ return TRUE; }
	virtual BOOL Shutdown()	{ return TRUE; }
	virtual BOOL Frame()	{ return TRUE; }
};
static CApplication *g_Application = NULL;
static long FAR PASCAL AppWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
//**************************************************************************************
/*static INT64 _FREQUENCY = 0;
static INT64 _FRAMECOUNT = 0;
static INT64 _FRAMERATE = 0;
static INT64 _TARGETFRAMERATE = 0; //Target Framerate

static INT64 _NEWCOUNT = 0;	
static INT64 _LASTCOUNT = 0;

static void SETFRAMERATE(INT64 framerate){ _TARGETFRAMERATE = framerate*1000; }
static int GETFRAMERATE(){	return _FRAMERATE; }

static HRESULT FRAMERATE_INIT(){
	SETFRAMERATE(10);
	QueryPerformanceFrequency((LARGE_INTEGER*)&_FREQUENCY);
	if(_FREQUENCY == 0){	
		OutputDebugString("This system does not support high resolution timers");
		return E_FAIL;
	}
	return S_OK;
}
static BOOL RegulateFrameCount(){
	return TRUE;
}
static void UpdateFrameCount(){
	QueryPerformanceCounter((LARGE_INTEGER*)&_NEWCOUNT);
	if(_NEWCOUNT == 0)
		::OutputDebugString("This System Doesnt Support High Resolution Timers");	
	_FRAMECOUNT++;
	if(_NEWCOUNT - _LASTCOUNT >= _FREQUENCY)
	{	_FRAMERATE = _FRAMECOUNT;
		_FRAMECOUNT=0;
		_LASTCOUNT=_NEWCOUNT;
	}
}
*/

//**************************************************************************************
#define REGULATEFRAME() if(CFrameRate::get()->RegulateFrameCount())	return TRUE;

class CFrameRate{
	static CFrameRate *m_CFrameRateSingleton;
protected:
	static INT64 m_Frequency;
	static INT64 m_FrameCount;
	static INT64 m_FrameRate;
	static INT64 NewCount;	
	static INT64 LastCount;
public:
	static CFrameRate *get(){
		if(m_CFrameRateSingleton == NULL){
			m_CFrameRateSingleton = new CFrameRate;
		}
		return m_CFrameRateSingleton;
	}
	~CFrameRate(){
		
	}
	HRESULT Init(){
		QueryPerformanceFrequency((LARGE_INTEGER*)&m_Frequency);
		if(m_Frequency == 0){	
			/****************************************/
			//::OutputDebugString("This system does not support high resolution timers");
			return E_FAIL;
		}
		return S_OK;
	}
	BOOL RegulateFrameCount(){
		static INT64 last = 0;
		QueryPerformanceCounter((LARGE_INTEGER*)&NewCount);
		if(NewCount - last > 60*1000){
			last = NewCount;
			return FALSE;
		}
	/*	while(TRUE)
	{	QueryPerformanceCounter((LARGE_INTEGER*)&CurrentTime);
		if((CurrentTime-StartTime)>(INT64)SecondsDelay)
			break;
	}*/
		return TRUE;
	}
	void UpdateFrameCount(){
		QueryPerformanceCounter((LARGE_INTEGER*)&NewCount);
		if(NewCount==0)
		{	
			//::OutputDebugString("This System Doesnt Support High Resolution Timers");	
		}
		m_FrameCount++;
		if(NewCount - LastCount >= m_Frequency)
		{	m_FrameRate = m_FrameCount;
			m_FrameCount=0;
			LastCount=NewCount;
		}
	}
	//float GetTicksPerMs(){	return ((float)m_Frequency/1000.0f);	}
	//void SetFrameRate(int millisecs);
	int GetFrameRate(){
		return (int)m_FrameRate;
	}
};

#endif