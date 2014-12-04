#include "GameEngineUtil.h"
/******************************
*	CEntry
*******************************/
CEntry::CEntry(){	m_bSuccesful = FALSE;						}
CEntry::CEntry(char *pText){	
	m_bSuccesful = FALSE; 
	SetText(pText);					
} 
CEntry::CEntry(CEntry &aEntry){	
	m_bSuccesful = FALSE;
	SetText(aEntry.GetText());		
}
CEntry::~CEntry(){}
char *CEntry::GetText(){	return (char*)m_pszText.c_str();	}
void CEntry::SetText(char *pText){	m_pszText = pText;			}
void CEntry::AddText(char pText){	m_pszText += pText;			}
void CEntry::Clear(){	m_pszText = "";							}
void CEntry::DelChar(){	
	if(!m_pszText.empty())
		m_pszText.erase(m_pszText.size()-1, 1);
}
/**********************************
	CConsole
**********************************/
CConsole::CConsole(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
				   char *pImgFilename, EventDispacher *pDispatch)
	: m_Font(pDevice, pSprite), m_bEnabled(FALSE), 
	  m_Sprite(pDevice, pImgFilename){	
	m_pDispatch = pDispatch;
	m_Sprite.AutoCenter();
	m_Font.SetColor(D3DCOLOR_XRGB(255,0,0));	// SetFont Color
	m_ColorFail = D3DCOLOR_XRGB(255, 0, 0);
	m_ColorGood = D3DCOLOR_XRGB(0,0,255);
	m_ColorCurrent = D3DCOLOR_XRGB(255, 255, 255);
}
CConsole::~CConsole(){ m_vPrevEntry.clear();	}
void CConsole::Activate(){
	static int timeLength = 10000 / 60, lastTime = 0;
	if(GetTickCount() - lastTime < timeLength)	
		return;
	lastTime = GetTickCount();
	m_bEnabled = (m_bEnabled) ? FALSE : TRUE;
}
void CConsole::SetActivate(BOOL bEnabled){  m_bEnabled = bEnabled;	}
BOOL CConsole::GetActivate(){ 	return m_bEnabled; }
void CConsole::Frame(IDirect3DDevice9 *pDevice){
	if(m_bEnabled){
		m_Sprite.Render(pDevice);
		m_Font.Begin();
		RenderText();		// Renders Text
		m_Font.End();
	}
}
void CConsole::RenderText(){
	//	Render Current Command
	SetText(&m_pCurrEntry);
	float t_y = (m_Sprite.getHeight() - m_Font.GetSize()->bottom);
	m_Font.SetTranslation(m_Sprite.getX(), t_y);
	m_Font.Render();

	int i = m_vPrevEntry.size()-1;
	m_Font.SetTranslation(m_Sprite.getX(), m_Sprite.getHeight() - 12);
	while(i > -1){
		//SetText(m_vPrevEntry[i]);
		m_Font.SetColor((m_vPrevEntry[i]->GetSuccessful()) ? m_ColorGood : m_ColorFail);
		m_Font.SetText(m_vPrevEntry[i]->GetText());
		m_Font.SetTranslationRel(0, -m_Font.GetSize()->bottom);
		m_Font.Render();
		i--;
	}
	m_Font.SetColor(m_ColorCurrent);

}
void CConsole::MonitorWndProc(HWND hWnd, UINT uMsg, 
					  WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_KEYDOWN: if(wParam == VK_F1) Activate(); 	return;
		case WM_CHAR:	 ListenKeyboardInput((char)wParam);	return;
		case WM_KEYUP:	 return;
	}
}
void CConsole::ListenKeyboardInput(char vkChar){
	//	Talk to WndProc controls curr line
	if(m_bEnabled)
		switch(vkChar){
			case VK_BACK: OnBackspace(); break;
			case VK_RETURN:	OnReturn();  break;
			case VK_TAB: 
				if(m_prevEntries[SUCESSFULCMD].size() != 0){
					static int commandnumber = 0;
					commandnumber++;
					if(commandnumber == m_prevEntries[SUCESSFULCMD].size())
						commandnumber = 0;
					m_pCurrEntry.SetText(m_prevEntries[SUCESSFULCMD][commandnumber]->GetText());
				}
			break;
			default: OnListen(vkChar);
	}
}
void CConsole::SetText(CEntry *pEntry){	
	m_Font.SetText(pEntry->GetText());	
}
inline void CConsole::OnBackspace(){	m_pCurrEntry.DelChar();	}
inline void CConsole::OnReturn(){
	ProcessCommand(new CEntry(m_pCurrEntry.GetText()));
}
inline void CConsole::OnListen(char &vkChar){
	/* Listen to Alpha Numeric Key Presses */
	m_pCurrEntry.AddText(vkChar);
}
void CConsole::LoadScriptFile(char *pFilename){
	ifstream ss(pFilename, ios::in);
	char *tLineCmd = new char [512];
	while(!ss.getline(tLineCmd, 512).eof()){
		if(strncmp(tLineCmd, "//", 2) == 0) // skip a line
			continue;
		ProcessCommand(new CEntry(tLineCmd));
	}
	delete [] tLineCmd;
	ss.close();
}
void CConsole::ProcessCommand(CEntry *aEntry){
	if(strlen(aEntry->GetText())>0){ 
		// Allocate a new CCommand Object 
		CEntry *pEntry = new CEntry(*aEntry);
		m_vPrevEntry.push_back(pEntry);
		if(m_pDispatch->ProcessCommand(aEntry->GetText()) != TRUE){
			m_vPrevEntry.push_back(new CEntry(UNKNOWNCMD));
			m_prevEntries[UNKNOWNCMD].push_back(aEntry);
		}
		else {		
			pEntry->SetSuccessful(TRUE);
			m_vPrevEntry.push_back(new CEntry(SUCESSFULCMD));
			m_prevEntries[SUCESSFULCMD].push_back(aEntry);
		} 
		m_pCurrEntry.Clear();
	}
}