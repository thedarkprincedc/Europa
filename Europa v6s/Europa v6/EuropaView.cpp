#include "EuropaView.h"
ID3DXSprite *g_pSprite; // DEBUG REMOVE
EuropaView::EuropaView(EuropaLogic *pEuropaLogic)
	:	m_pEuropaLogic(pEuropaLogic){	}
EuropaView::~EuropaView(void){	}
void EuropaView::Init(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
	mFont = new CFontEx(pDevice, pSprite);
	//pBackgound = new CBackground(pDevice, "./resources/starfield.jpg", -4, 0);
	pBackgound = new CBackground(pDevice, "./resources/rdx_starfield.png", -4, 0);
	//pBackgound->Load(pDevice, "./resources/starfield.jpg", -4, 0);
	pEntityManager = new CEntityManager(pDevice, m_pEuropaLogic->GetEntities());
	m_pConsole = new CConsole(pDevice, pSprite, "./Resources/phoenix_console.jpg", 
			m_pEuropaLogic->GetEventDispatcher());
	pSound = new CSound();
	m_pEuropaLogic->GetEventDispatcher()->AddEventListener("ENTITY", pEntityManager);
	m_pEuropaLogic->GetEventDispatcher()->AddEventListener("SOUND", pSound);
	m_pEuropaLogic->GetEventDispatcher()->AddEventListener("BACKGROUND", pBackgound);
	m_pEuropaLogic->GetEventDispatcher()->AddNewCommand("CreateEntity", "ENTITY");
	m_pEuropaLogic->GetEventDispatcher()->AddNewCommand("SetTransColor", "ENTITY");
	m_pEuropaLogic->GetEventDispatcher()->AddNewCommand("LoadSong", "SOUND");
	m_pEuropaLogic->GetEventDispatcher()->AddNewCommand("ChangeBackground", "BACKGROUND");
	m_pEuropaLogic->GetEventDispatcher()->AddNewCommand("DeleteAllEnt", "ENTITY");
	m_pConsole->LoadScriptFile("C:\\Documents and Settings\\Brett\\My Documents\\Visual Studio 2005\\Projects\\Europa v6\\Europa v6\\helpfile.txt");
	pDebugLogic = new DebugLogic(pDevice, pSprite);// DEBUG REMOVE
	g_pSprite= pSprite; // DEBUG REMOVE
}

void EuropaView::ListenToControls(){
	ENTITYMAP::iterator mapIter;
	if((mapIter = m_pEuropaLogic->GetEntityMap().find("PLAYABLECHARACTERS")) != 
		m_pEuropaLogic->GetEntityMap().end()){
			CSprite *pSprite = (CSprite*)mapIter->second[0];
			// Add Controls
	}	
}
void EuropaView::MonitorWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
		m_pConsole->MonitorWndProc(hWnd, uMsg, wParam, lParam);
}

void EuropaView::Render(IDirect3DDevice9 *pDevice){
	ENTITYMAP::iterator mapIter;
	RenderBackground(pDevice);
	pEntityManager->Render(pDevice);
	g_pSprite->Begin(0);// DEBUG REMOVE
	pDebugLogic->PrintDebugMessages(m_pEuropaLogic);// DEBUG REMOVE
	g_pSprite->End();// DEBUG REMOVE
	m_pConsole->Frame(pDevice);
	if(::GetAsyncKeyState(VK_SPACE))
		this->m_pEuropaLogic->GetEventDispatcher()->SendEvent("ENTITY", "CreateEntity ./Resources/plane.png BulletAi 0 66");
	
	/*
	for(mapIter = m_pEuropaLogic->GetEntityMap().begin();
		mapIter != m_pEuropaLogic->GetEntityMap().end(); mapIter++){
		for(int i = 0; i < mapIter->second.size(); i++)
			mapIter->second[i]->Render(pDevice);
	}
	*/
}
void EuropaView::RenderBackground(IDirect3DDevice9 *pDevice){
	pBackgound->Frame(pDevice);
}
void EuropaView::RenderPlayerHud(IDirect3DDevice9 *pDevice){
	// Debug HUD 
	string p;
	sprintf((char*)p.c_str(),"%s %d %s %d",
		"Score",m_pEuropaLogic->GetScore(),
		"\nEntities", pEntityManager->GetNumOfEntities());
	//mFont->SetText((char*)p.c_str());
	string g = pSound->GetStatusText();
	mFont->SetText((char*)g.c_str());
	mFont->Render();
	// User HUD
}