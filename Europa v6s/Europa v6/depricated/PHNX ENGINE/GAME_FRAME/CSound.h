#ifndef _CSOUND_H_
#define _CSOUND_H_
#include <dshow.h>
#include <initguid.h>
#pragma comment (lib, "strmiids.lib")
#define MATCH(STRINGA, STRINGB) if(stricmp(STRINGA, STRINGB) == 0)
/*****************************************
*	CSound - Uses Direct Show Filters	 *
*			 to Play Music				 *
*****************************************/
class CSound : public EventListener{
public:
	CSound();							// Constructor
	CSound(char *pszSongName);			// 
	virtual ~CSound();					// Destructor
	HRESULT Load(char *pszSongName);		// Load Sound
	void Play();						// Play from beginning
	void Play(double position);			// Play from a position
	double Stop();						// Stop
	void SetPosition(double position);	// Set Position
	inline double GetDuration();		// Get Duration
	inline double GetPosition();		// Get Position
	const char *GetFilenameText();		// Get Filename Text
	const char *GetDurationText();		// Get Duration Text
	const char *GetPositionText();		// Get Position Text
	string GetStatusText();		// Get Status
	BOOL ProcessEvent(IDirect3DDevice9 *pDevice, Event *pEvent);
	BOOL Playing()
	{
		// Error checking
		if(m_MediaEvent == NULL)
		return FALSE;

		// Get event and handle it
		long Event, Param1, Param2;
		m_MediaEvent->GetEvent(&Event, &Param1, &Param2, 1);
		if(Event == EC_COMPLETE) {
			m_MediaEvent->FreeEventParams(Event, Param1, Param2);
			return TRUE;
		}
		m_MediaEvent->FreeEventParams(Event, Param1, Param2);

		return FALSE;
	}
protected:	
	void Init();							// Initialization Funct
	IGraphBuilder		*m_GraphBuilder;	// Graph Builder 
	IMediaControl		*m_MediaControl;	// Media Control
    IMediaEventEx		*m_MediaEvent;		// Media Event;
	IMediaPosition		*m_MediaPosition;	// Media Position
private:
	string			    m_pFilename;		// Filename
	double				m_duration;			// Duration
	double				m_position;			// Position
	double				m_stoptime;			// Stop Time
};
#endif