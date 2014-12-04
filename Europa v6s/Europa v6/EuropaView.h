#pragma once
#include "../PHNX ENGINE/phoenixengine.h"
#include "./EuropaLogic.h"
#include <sstream>
using namespace std;
class DebugLogic{
public:
	DebugLogic(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
		pFont = new CFontEx(pDevice, pSprite);
	}
	void PrintDebugMessages(EuropaLogic *pLogic){
		vector<CEntity*>::iterator pEntities = pLogic->GetEntities()->begin();
		char s[256];
		pFont->SetTranslation(0, 0);
		int i = 0;
		int pos_x = 0;
		int pos_y = 0;
		while(pEntities != pLogic->GetEntities()->end()){
			sprintf(s, "Entity %d \n X: %d \n Y: %d \n Width: %d \n Height: %d \n", i, 
				(int)(*pEntities)->GetSprite()->getX(),
				(int)(*pEntities)->GetSprite()->getY(),
				(int)(*pEntities)->GetSprite()->getWidth(), 
				(int)(*pEntities)->GetSprite()->getHeight());
			pFont->SetText(s);
			pFont->Render();
			pEntities++;
			pos_x += 75;
			i++;
			if(pos_x >= 700){
				pos_x = 0;
				pos_y += 70;
			}
			pFont->SetTranslation(pos_x, pos_y);
			
		}
	}
protected:
	CFontEx *pFont;
};
class EuropaView {// Screen Rendering
public:
	EuropaView(EuropaLogic *pEuropaLogic);
	~EuropaView(void);

	void Init(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite);
	void ListenToControls();
	void MonitorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Render(IDirect3DDevice9 *pDevice);
	void RenderBackground(IDirect3DDevice9 *pDevice);
	void RenderPlayerHud(IDirect3DDevice9 *pDevice);
protected:
	CFontEx			*mFont;
	EuropaLogic		*m_pEuropaLogic;
//  Level
	CBackground		*pBackgound;
	CEntityManager	*pEntityManager;
	CSound			*pSound;
//  Debugger
	CConsole		*m_pConsole;
// Debug Module
	DebugLogic		*pDebugLogic;
};
class LevelManager{
	//typedef struct{
	//	string LevelCommand;
	//	string Background;
	//};
};
