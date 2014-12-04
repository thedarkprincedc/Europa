/********************************
*	CConsole.h					*
*	Phoenix.Net Console Class	*
*	BrettMosley@hotmail.com		*
********************************/
#ifndef _CCONSOLE_H_
#define _CCONSOLE_H_
#include "./GameEngineUtil.h"
//	Defines
#define CENTER(ScreenDimension, TextureDimension) ((ScreenDimension - TextureDimension)/2)
#define MATCH(STRINGA, STRINGB) if(stricmp(STRINGA, STRINGB) == 0)
#define SUCESSFULCMD "Command Successful"
#define UNKNOWNCMD	 "Unknown Command"
/**
	CEntry Acounts for one line of text the console
*/
class CEntry{
public:
	CEntry();					// Constructor
	CEntry(char *pText);
	CEntry(CEntry &aEntry);
	~CEntry();					// Destructor
	char *GetText();	
	void SetText(char *pText);						
	void AddText(char pText);
	void Clear();
	void DelChar();
	BOOL GetSuccessful(){	return m_bSuccesful;	}
	void SetSuccessful(BOOL bSuccess){	m_bSuccesful = bSuccess;	}
protected:
	string		m_pszText;
	BOOL		m_bSuccesful;	
};
/**
	Command Console 
*/
class CConsole {
public:
	//CConsole(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,
	//	D3DCOLOR color, float width, float height, 
	//	EventDispacher *pDispatch = NULL);
	CConsole(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
		char *pImgFilename, EventDispacher *pDispatch = NULL);	
	virtual ~CConsole();
	void MonitorWndProc(HWND hWnd, UINT uMsg, 
		WPARAM wParam, LPARAM lParam);
	void ListenKeyboardInput(char vkChar);	//
	void Frame(IDirect3DDevice9 *pDevice);	// Display Frame
	void Activate();						// Activate
	void SetActivate(BOOL bEnabled = TRUE);	// SetActivate
	BOOL GetActivate();						// GetActivate
	void LoadScriptFile(char *pFilename);	//
private:
	void ProcessCommand(CEntry *aEntry);	//
	void SetText(CEntry *pEntry);			// SetText
	void RenderText();						// RenderText
	inline void OnBackspace();				// OnBackspace
	inline void OnReturn();					// OnReturn 
	inline void OnListen(char &vkChar);		// OnListen
protected:
	CEntry						  m_pCurrEntry;	 // Current Entry
	vector<CEntry*>				  m_vPrevEntry;	 // Previous Entry Sequence
	map<string, vector<CEntry*> > m_prevEntries; // Previous Success / Fail
protected:
	CSprite				m_Sprite;		// CSprite
	CFontEx				m_Font;			// Font
	BOOL				m_bEnabled;		// Enabled/Disabled
	EventDispacher	   *m_pDispatch;	// Dispather
	D3DCOLOR			m_ColorFail;
	D3DCOLOR			m_ColorGood;
	D3DCOLOR			m_ColorCurrent;
};
#endif