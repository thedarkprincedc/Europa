/********************************
*	CConsole.h					*
*	Phoenix.Net Console Class	*
*	BrettMosley@hotmail.com		*
********************************/
#ifndef _CCONSOLE_H_
#define _CCONSOLE_H_

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include "./CSprite.h"
#include "./CFont.h"
using namespace std;


//	Defines
#define CENTER(ScreenDimension, TextureDimension) ((ScreenDimension - TextureDimension)/2)
#define MATCH(STRINGA, STRINGB) if(stricmp(STRINGA, STRINGB) == 0)
#define MAX_LINES 15
#define CMD_SUCESSFUL 0x0001
#define CMD_UNSUCESSFUL 0x0002
#define CMD_UNKNOWN 0x0000
#define CMDCOLOR_FAIL D3DCOLOR_XRGB(255, 0, 0)
#define	CMDCOLOR_GOOD D3DCOLOR_XRGB(0,0,255)
#define	CMDCOLOR_CURR D3DCOLOR_XRGB(255, 255, 255)
typedef BOOL (*PFNCMDPARSERCALLBACK)(char *);

typedef struct CEntryEx{
	CEntryEx(){	x = y = 0;}
	CEntryEx(const string &aString, int ix = 0, int iy = 0){
		m_pszText += aString;
		x = ix;
		y = iy;
		m_dwSucessful = CMD_UNKNOWN;
	}
	string m_pszText;
	float x, y;
	DWORD m_dwSucessful;
};
class CConsole {
public:
	CConsole(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, char *pImgFilename,
		PFNCMDPARSERCALLBACK pfnCommandParserCallback = NULL)
			: m_Font(pDevice, pSprite), m_bEnabled(FALSE), m_Sprite(pDevice, pImgFilename){	
		m_Sprite.SetTranslationAbs(CENTER(1024, m_Sprite.GetWidth()), 0); 
		m_Font.SetColor(CMDCOLOR_CURR);	// SetFont Color
		m_pCurrEntry.x = m_Sprite.GetRect().left;
		m_pCurrEntry.y = (m_Sprite.GetHeight() - m_Font.GetRect(pSprite).bottom);
		m_pfnCommandParser = pfnCommandParserCallback;
		m_CommandNumber = 0;
	}
	virtual ~CConsole(){
	
	}
	void MonitorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		switch(uMsg){
		case WM_KEYDOWN: if(wParam == VK_F1) Activate();    return;
		case WM_KEYUP:	 return;
		case WM_CHAR:	 ListenKeyboardInput((char)wParam);	return;
		}
	}
	inline void OnBackspace(){
		if(m_pCurrEntry.m_pszText.length() > 0)
			m_pCurrEntry.m_pszText.erase((m_pCurrEntry.m_pszText.length()-1),1);
	}
	inline void OnReturn(){
		if(m_pCurrEntry.m_pszText.size() > 0){
			int i = 1;	
			CEntryEx *pEntity = new CEntryEx(m_pCurrEntry.m_pszText, m_pCurrEntry.x, (m_pCurrEntry.y-(12*i)));
			m_vPrevEntry.push_back(pEntity);
			if(m_pfnCommandParser != NULL){
				// Process Comand
				pEntity->m_dwSucessful = (m_pfnCommandParser((char *)pEntity->m_pszText.c_str())) ? CMD_SUCESSFUL : CMD_UNSUCESSFUL;
			}
			for(int j = (m_vPrevEntry.size()-1); j != -1; j--){
				m_vPrevEntry[j]->x = m_pCurrEntry.x;
				m_vPrevEntry[j]->y = ( m_pCurrEntry.y - (12*i) );
				i++;
			}
			m_pCurrEntry.m_pszText = "";
		}		
	}
	inline void OnListen(char &vkChar){
		// Listen to Alpha Numeric Key Presses 
		m_pCurrEntry.m_pszText += vkChar;
	}
	void ProcessCommand(string pString){
		if(pString.size() > 0){
			int i = 1;	
			CEntryEx *pEntity = new CEntryEx(pString, m_pCurrEntry.x, (m_pCurrEntry.y-(12*i)));
			m_vPrevEntry.push_back(pEntity);
			if(m_pfnCommandParser != NULL){
				// Process Comand
				pEntity->m_dwSucessful = (m_pfnCommandParser((char *)pEntity->m_pszText.c_str())) ? CMD_SUCESSFUL : CMD_UNSUCESSFUL;
			}
			for(int j = (m_vPrevEntry.size()-1); j != -1; j--){
				m_vPrevEntry[j]->x = m_pCurrEntry.x;
				m_vPrevEntry[j]->y = ( m_pCurrEntry.y - (12*i) );
				i++;
			}
			m_pCurrEntry.m_pszText = "";
		}
	}
	void LoadScriptFile(char *pFilename){
		ifstream ss(pFilename, ios::in);
		char *tLineCmd = new char [512];
		while(!ss.getline(tLineCmd, 512).eof()){
			if(strncmp(tLineCmd, "//", 2) == 0) // skip a line
				continue;
			ProcessCommand(tLineCmd);
		}
		delete [] tLineCmd;
		ss.close();
	}
	
	void RenderText(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
		for(int i = 0; i < m_vPrevEntry.size(); i++){
			m_Font.SetText((char*)m_vPrevEntry[i]->m_pszText.c_str());
			switch(m_vPrevEntry[i]->m_dwSucessful){
				case CMD_SUCESSFUL: m_Font.SetColor(CMDCOLOR_GOOD); break;
				case CMD_UNSUCESSFUL: m_Font.SetColor(CMDCOLOR_FAIL); break;
				case CMD_UNKNOWN:	m_Font.SetColor(CMDCOLOR_CURR);	break;
			}
			m_Font.SetTranslationAbs(m_vPrevEntry[i]->x, m_vPrevEntry[i]->y);
			m_Font.Render(pDevice, pSprite);	
		}
		m_Font.SetColor(CMDCOLOR_CURR);	
		m_Font.SetText((char*)m_pCurrEntry.m_pszText.c_str());
		m_Font.SetTranslationAbs(m_pCurrEntry.x, m_pCurrEntry.y);
		m_Font.Render(pDevice, pSprite);
	}
	void ListenKeyboardInput(char vkChar){
		//	Talk to WndProc controls curr line
		if(m_bEnabled)
			switch(vkChar){
				case VK_BACK:   OnBackspace(); break;
				case VK_RETURN:	OnReturn();    break;
				case VK_TAB: 
					if(m_vPrevEntry.size() != 0){
						m_CommandNumber++;
						if(m_CommandNumber == m_vPrevEntry.size()){
							m_CommandNumber = 0;
						}
						m_pCurrEntry.m_pszText = m_vPrevEntry[m_CommandNumber]->m_pszText;
					}
				break;
				default: OnListen(vkChar);
		}
	}
	virtual void Frame(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
		if(m_bEnabled){
			m_Sprite.Render(pDevice, pSprite);
			RenderText(pDevice, pSprite);		// Renders Text
		}
	}
	virtual void Activate(){					   // Activate
		static int timeLength = 10000 / 60, lastTime = 0;
		if(GetTickCount() - lastTime < timeLength)	
			return;
		lastTime = GetTickCount();
		m_bEnabled = (m_bEnabled) ? FALSE : TRUE;
	}
	void SetActivate(BOOL bEnabled = TRUE){  m_bEnabled = bEnabled;	}
	BOOL GetActivate(){ 	return m_bEnabled; }
protected:
	CEntryEx						  m_pCurrEntry;	 // Current Entry
	vector<CEntryEx*>				  m_vPrevEntry;	 // Previous Entry Sequence
	map<string, vector<CEntryEx*> >   m_mvPrevEntry; // Previous Success / Fail
	int							  m_CommandNumber;
protected:
	CSpriteDx			m_Sprite;		// CSprite
	CFontEx				m_Font;			// Font
	BOOL				m_bEnabled;		// Enabled/Disabled
	PFNCMDPARSERCALLBACK m_pfnCommandParser;
};
class CDropDownConsole : public CConsole {	
public:
	CDropDownConsole(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, char *pImgFilename,
			PFNCMDPARSERCALLBACK pfnCommandParserCallback = NULL)
		:	CConsole(pDevice, pSprite, pImgFilename, pfnCommandParserCallback){
		m_Sprite.SetTranslationRel(0, -m_Sprite.GetHeight());
		m_Animate = ANIMATE_DOWN;
		m_bShowText = FALSE;
	}
	void Frame(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
		m_Sprite.Render(pDevice, pSprite);
		if(m_bShowText)
			RenderText(pDevice, pSprite);		
		m_Animate = (m_bEnabled) ? ANIMATE_DOWN : ANIMATE_UP;
		switch(m_Animate){
			case ANIMATE_DOWN:
				if(m_Sprite.GetYPos() != 0){
					m_nDirectionY = 2;	
					m_Sprite.SetTranslationRel(0, m_nDirectionY);
				}
				else m_bShowText = TRUE;
			break;
			case ANIMATE_UP:
				if(m_Sprite.GetYPos() != -m_Sprite.GetHeight()){
					m_nDirectionY = -2;	
					m_Sprite.SetTranslationRel(0, m_nDirectionY);
				}
				else m_bShowText = FALSE;
			break;
		}
	}
	/*
	void Frame(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
		// Using Linear Interpolation 
		//(d1 - d2)*t = d
		//static float starty endy = 0;
		static float t = 0;
		m_Sprite.SetTranslationAbs(0, (256 - 0)*t);
		m_Sprite.Render(pDevice, pSprite);
	}*/
protected:
	enum {ANIMATE_UP, ANIMATE_DOWN} m_Animate;
	BOOL m_bShowText;
	float m_nDirectionY;
};
#endif