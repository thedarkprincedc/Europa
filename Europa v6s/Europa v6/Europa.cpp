/* Libraries */
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "./Core_Graphics.h"
#include "./CSprite.h"
#include "./CFont.h"
#include "./CBackground.h"
#include "./CConsole.h"
#include "./UserInterface.h"
#include "./CStatemanager.h"
#include "./fmodwrapper.h"
#include <vector>
using namespace std;

#define TITLEMUSIC "./Resources/Sound/Great.wma"
#define TITLEBKG "./Resources/atmosphere.jpg"
#define LASER_SOUND "./Resources/laser.wav"
#define ENEMYDEATH_SOUND "./Resources/explosion2.wav"
#define PLAYERDEATH_SOUND "./Resources/explosion.WAV"

CGraphics     *_pGraphics = new CGraphics();
CStateManager *_pStateManager = new CStateManager();
CBackground   *_pBackground;
UIPanel		  *_pPanel;

int Score = 0;
int ScoreAdd = 0;
char ScoreText[255];
#define ADD_RATE 10
void ScoreAnimation(){
	if(ScoreAdd > 0){
		Score += ADD_RATE;
		ScoreAdd-=ADD_RATE;
	}
}

typedef struct CEntityEx{
	int m_MaxHealth;
	int m_CurHealth;
	CSpriteDx *pSprite;
	typedef void (*PAIFUNCT)();
	void DoAiFunct();
};
CEntityEx *ADDENTITY(CEntityEx *pEntity, char *pTextureName, D3DCOLOR alphacolor, int x, int y, int MaxHealth, int CurHealth){
	pEntity->pSprite = new CSpriteDx(_pGraphics->GetD3DDevice(), pTextureName, alphacolor);
	pEntity->m_MaxHealth = MaxHealth;
	pEntity->m_CurHealth = CurHealth;
	pEntity->pSprite->SetTranslationAbs(x, y);
	return pEntity;
}

void RENDERENTITYS(const vector<CEntityEx*> &pEntity){
	for(int i = 0; i < pEntity.size(); ++i)
		pEntity[i]->pSprite->Render(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
}
// VECTORS
static vector<CEntityEx*> _vNPC;
static vector<CEntityEx*> _vPC;
static vector<CEntityEx*> _vNPCProjectiles;
static vector<CEntityEx*> _vPCProjectiles;
static vector<CEntityEx*> _vNonCollide;
static vector<CEntityEx*> _vNPCBoss;
vector<int> SenseDistance(const CEntityEx *target, const CEntityEx *entity){
	vector<int> p;
	int xprime = (entity->pSprite->GetXPos() - target->pSprite->GetXPos())/32;
	int yprime = (entity->pSprite->GetYPos() - target->pSprite->GetYPos())/target->pSprite->GetHeight();
	p.push_back(xprime);
	p.push_back(yprime);
	return p;
}
void BossAi(CEntityEx *entity){
	vector<int> p;
	// Check for missiles
	for(int i = 0; i < _vPCProjectiles.size(); ++i){
		p = SenseDistance(entity, _vPCProjectiles[i]);
		// if one is close enough to something
		//if(p[1] == 0){
		//	RECT rc = entity->pSprite->GetRect();
		//	entity->pSprite->SetTranslationRel(0,-1);
		//}
	}

}
void GAME_TITLE(void *Ptr, long Purpose);
void GAME_STATE(void *Ptr, long Purpose);
BOOL CommandParser(char *pStringCommand);
void CHECKCOLLISION(){
	RECT rcInter;
	for(int i = 0; i < _vPC.size(); i++){
		for(int j = 0; j < _vNPC.size(); j++){
			if(::IntersectRect(&rcInter, &_vPC[i]->pSprite->GetRect(), &_vNPC[j]->pSprite->GetRect())){	
				_vNPC.erase(_vNPC.begin()+j);
				_vPC[i]->m_CurHealth--;
				if(_vPC[i]->m_CurHealth == 0)
					CommandParser("GAME_RESET");
				PLAYSOUND_FMOD(PLAYERDEATH_SOUND);
				return;
			}
		}
	}
	for(int i = 0; i < _vPCProjectiles.size(); i++){ // PLAYER PROJECTILE
		for(int j = 0; j < _vNPC.size(); j++){
			if(::IntersectRect(&rcInter, &_vPCProjectiles[i]->pSprite->GetRect(), &_vNPC[j]->pSprite->GetRect())){	
				_vNPC.erase(_vNPC.begin()+j);
				_vPCProjectiles.erase(_vPCProjectiles.begin()+i);
				ScoreAdd+= 100;
				char pChar[255];
				int x = rcInter.left + (rcInter.left - rcInter.right);
				int y = rcInter.top + (rcInter.top - rcInter.bottom); 
				sprintf(pChar,"ADDENTITY ./Resources/explode_fire.png NC %d %d 10 10", x, y);
				CommandParser(pChar);
				PLAYSOUND_FMOD(ENEMYDEATH_SOUND);
				return;
			}
		}
	}
}

void GAME_TITLE(void *Ptr, long Purpose){
	static CSpriteDx *pSprite = new CSpriteDx(_pGraphics->GetD3DDevice(), "./Resources/title.png", D3DCOLOR_XRGB(255,0,255));
	switch(Purpose){
		case INITPURPOSE:
			_pBackground = new CBackground(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject(), TITLEBKG);
			LOADSOUND_FMOD(TITLEMUSIC, STREAM);
			PLAYSOUND_FMOD(TITLEMUSIC);
			_pPanel = new UIPanel(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject(), (1024 - 300)/2, 400, CommandParser);
			_pPanel->AddTextElement("NEW GAME", 0, 0, "PUSH_STATE GAMESTATE");
			_pPanel->AddTextElement("EXIT GAME", 0, 60, "EXIT");
			return;
		case SHUTDOWNPURPOSE:
			delete pSprite;
			return;
		case FRAMEPURPOSE:
			_pGraphics->BeginSprite();
			_pBackground->Frame(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
			pSprite->Render(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
			_pPanel->RenderElements();
			_pGraphics->EndSprite();
			return; 
	}
}
void GAME_CONTINUE(void *Ptr, long Purpose){
		switch(Purpose){
		case INITPURPOSE:
			//"Are you sure youd like to continue"
			_pPanel->AddTextElement("YES",0,60, "POP_STATE");
			_pPanel->AddTextElement("NO",0,60, "EXIT");
			return;
		case SHUTDOWNPURPOSE:
			return;
		case FRAMEPURPOSE:
			_pGraphics->BeginSprite();
			_pBackground->Frame(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
			_pPanel->RenderElements();
			_pGraphics->EndSprite();
			return; 
	}
}
#define ENEMYSPAWNTIMEOUT 400
#define MAX_ENEMIES 10
void ENEMY_SPAWNER(){
	//TEMPORARY MOVEMENT
	for(int i = 0; i < _vNPC.size(); ++i){
		_vNPC[i]->pSprite->Render(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
		_vNPC[i]->pSprite->SetTranslationRel(-7, 0);
	}
	//******************
	static long LastShotFired = 0;
	if((GetTickCount() - LastShotFired) < ENEMYSPAWNTIMEOUT)
		return;
	LastShotFired = GetTickCount();
	if(_vNPC.size() == MAX_ENEMIES)
		return;
	char p[128];
	sprintf(p, "AddEntity ./Resources/tigersharktrans.BMP NPC %d %d", rand()%640+620, rand()%480);
	CommandParser(p);
}
#define GENERICLASERTIMEOUT 240
void GAMECONTROLS(const vector<CEntityEx*> &pEntity){
	for(int i = 0; i < _vPCProjectiles.size(); ++i){
		_vPCProjectiles[i]->pSprite->Render(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
		_vPCProjectiles[i]->pSprite->SetTranslationRel(9, 0);
	}
	if(pEntity.size() > 0){
		if(::GetAsyncKeyState(VK_LEFT))		pEntity[0]->pSprite->SetTranslationRel(-3, 0);
		if(::GetAsyncKeyState(VK_RIGHT))	pEntity[0]->pSprite->SetTranslationRel(3, 0);
		if(::GetAsyncKeyState(VK_UP))		pEntity[0]->pSprite->SetTranslationRel(0, -3);
		if(::GetAsyncKeyState(VK_DOWN))		pEntity[0]->pSprite->SetTranslationRel(0, 3);
		if(::GetAsyncKeyState(VK_SPACE)){
			static long LastShotFired = 0;
			if((GetTickCount() - LastShotFired) < GENERICLASERTIMEOUT)
				return;
			LastShotFired = GetTickCount();
			char p[128];
			D3DXVECTOR2 pos = pEntity[0]->pSprite->GetPositionVector2();
			sprintf(p, "AddEntity ./Resources/genericlaser.PNG PCPROJECTILE %f %f",	pos.x, pos.y);
			CommandParser(p);
			PLAYSOUND_FMOD(LASER_SOUND);
		}
	}
}
void PrintHealth(CSpriteDx *pHealthSpriteIcon, CEntityEx *pEntity, int x = 0, int y = 0){
	pHealthSpriteIcon->SetTranslationAbs(x, y);
	for(int i = 0; i < pEntity->m_CurHealth; i++){
		pHealthSpriteIcon->Render(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
		pHealthSpriteIcon->SetTranslationRel(pHealthSpriteIcon->GetWidth()+2, 0);
	}
}

typedef struct LevelStr{
	char *pLevelBackground;
	char *pLevelBackgroundSound;

	//float LevelScrollSpeedX;
	//float LevelScrollSpeedY;
	//char *pPlayerTexture;
};
LevelStr GetCurrentLevel();
void LoadLevel(char *pLevel){
	LevelStr *pLevelStr;

}
BOOL inGame = FALSE;
void GAME_STATE(void *Ptr, long Purpose){
	
	BOOL boss = FALSE;
	static CFontEx *pFont = new CFontEx(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject(), "Arial", 40);
	static CSpriteDx *pHealth = new CSpriteDx(_pGraphics->GetD3DDevice(), "./Resources/plane.png", D3DCOLOR_XRGB(243,0,0));
	switch(Purpose){
		case INITPURPOSE:
			inGame = TRUE;
			LOADSOUND_FMOD(LASER_SOUND);
			LOADSOUND_FMOD(ENEMYDEATH_SOUND);
			LOADSOUND_FMOD(PLAYERDEATH_SOUND);
			_pBackground->setDirectionAbs(-2.5, 0);
			_pBackground->SetModulateColorAbs(255, 255, 255, 255);
			CommandParser("Game_Reset");
			return;
		case SHUTDOWNPURPOSE:
			return;
		case FRAMEPURPOSE:
			_pGraphics->BeginSprite();
			//_pBackground->Frame(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
			// Score 
			sprintf(ScoreText ,"Score: %d", Score);
			pFont->SetText(ScoreText);
			pFont->SetTranslationAbs(400, 0);
			pFont->Render(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
			ScoreAnimation();
			GAMECONTROLS(_vPC);
			CHECKCOLLISION();
			if(Score > 2000)
				boss = TRUE;
			if(boss){
				static BYTE c = 255;

				if(c == 255)
  					CommandParser("ADDENTITY ./Resources/n24409432_20284.jpg BOSS 700 300");
				if(c != 75){
					_pBackground->SetModulateColorAbs(255,c,c,c);
					//_pBackground->SetModulateColorRel(-1 , -1, -1, -1);
					c--;
				}
				
				if(_vNPCBoss.size() > 0)
					BossAi(_vNPCBoss[0]);
				RENDERENTITYS(_vNPCBoss);
			}
			else
				ENEMY_SPAWNER();        
			//******************************
			if(_vPC.size()>0)
				PrintHealth(pHealth, _vPC[0]);
			
			RENDERENTITYS(_vPC);
			RENDERENTITYS(_vPCProjectiles);
			RENDERENTITYS(_vNPC);
			RENDERENTITYS(_vNPCProjectiles);
			RENDERENTITYS(_vNonCollide);
			_pGraphics->EndSprite();
			return; 
	}
}

CConsole *_pConsole;
void GAME_INIT(HWND &hWnd){
	_pGraphics->Init();
	_pGraphics->SetMode(hWnd, TRUE, TRUE, 1024, 768, 32);
	_pConsole = new CConsole(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject(),
								  "./Resources/phoenix_console.jpg", CommandParser);
	INIT_FMOD();
	_pGraphics->GetD3DDevice()->SetRenderState( D3DRS_ZENABLE,   FALSE );
	_pStateManager->Push(GAME_TITLE);
}
void GAME_LOOP(){	
	_pGraphics->Clear(0xffffffff);	//	Clear Display
	_pGraphics->BeginScene();		//	Begin Scene

	_pStateManager->Process();	
	_pGraphics->BeginSprite();
	_pConsole->Frame(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject());
	_pGraphics->EndSprite();
	_pGraphics->EndScene();			//	End Scene
	_pGraphics->Display();			//	Display Back Buffer
}
void GAME_SHUTDOWN(){	_pStateManager->PopAll();	}
void GAME_RESET(){
	_vNPC.clear();
	_vPC.clear();
	_vNPCProjectiles.clear();
	_vPCProjectiles.clear();
	CommandParser("ADDENTITY ./Resources/plane.png PC 20 300 10 10");
}
BOOL CommandParser(char *pStringCommand){
	char tmp[255];
	sscanf(pStringCommand, "%s", &tmp);
	MATCH(tmp, "ADDENTITY"){
		// ADDENTITY <FILENAME> <TYPE> <MASKCOLORXRGB> <X> <Y> <MAXHEALTH> <CURRHEALTH>
		char type[16]; float x = 0; float y = 0;
		int maxHealth = 0, currHealth = 0;
		sscanf(pStringCommand, "%*s %s %s %f %f %d %d", &tmp, &type, &x, &y, &maxHealth, &currHealth);
		MATCH(type, "PCPROJECTILE")		
			_vPCProjectiles.push_back(ADDENTITY(new CEntityEx(), tmp, 0, x, y, maxHealth, currHealth));
		MATCH(type, "NPCPROJECTILE")	
			_vNPCProjectiles.push_back(ADDENTITY(new CEntityEx(), tmp, 0, x, y, maxHealth, currHealth));
		MATCH(type, "PC")	
			_vPC.push_back(ADDENTITY(new CEntityEx(), tmp, 0, x, y, maxHealth, currHealth));
		MATCH(type, "NPC")	
			_vNPC.push_back(ADDENTITY(new CEntityEx(), tmp, D3DCOLOR_XRGB(0,0,255), x, y, maxHealth, currHealth));
		MATCH(type, "NC") 
			_vNonCollide.push_back(ADDENTITY(new CEntityEx(), tmp, D3DCOLOR_XRGB(255,0,255), x, y, maxHealth, currHealth));
		MATCH(type, "BOSS")  
			_vNPCBoss.push_back(ADDENTITY(new CEntityEx(), tmp, D3DCOLOR_XRGB(255,0,255), x, y, maxHealth, currHealth));
	}	
	MATCH(pStringCommand, "GAME_RESET")	
		GAME_RESET();
	MATCH(pStringCommand, "PUSH_STATE GAMESTATE")	
		_pStateManager->Push(GAME_STATE);
	MATCH(pStringCommand, "PUSH_STATE CONTINUE")	
		_pStateManager->Push(GAME_CONTINUE);
	MATCH(pStringCommand, "POP_STATE")	
		_pStateManager->Pop();
	MATCH(pStringCommand, "EXIT")	PostQuitMessage(0);
	return FALSE;
}
void OnEscape(WPARAM wParam){
	if(wParam == VK_ESCAPE){
		if(inGame == TRUE){
			CommandParser("POP_STATE");
			inGame = FALSE;
		}
		else
			CommandParser("EXIT");
	}
}
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ){
    switch( msg ){
		case WM_CREATE:								break;
        case WM_DESTROY:    PostQuitMessage( 0 );   break;
		case WM_KEYDOWN:	OnEscape(wParam);		break;
    }
	_pConsole->MonitorWndProc( hWnd, msg, wParam, lParam );
	_pPanel->TrackMouseInputProc(msg, wParam, lParam);
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT ){
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"D3D Tutorial", NULL };
    RegisterClassEx( &wc );
    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"Europa V6.3 beta www.rit.edu/btm6256 brettmosley@hotmail.com",
                              WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768, NULL, NULL, wc.hInstance, NULL );
    // Show the window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );
    // Enter the message loop
    MSG msg;
    ZeroMemory( &msg, sizeof(msg) );
	GAME_INIT(hWnd);
	while( msg.message!=WM_QUIT ){
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ){ // Active Program Loop
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		else if(REGULATE_FRAMERATE())
			GAME_LOOP();
    }
	GAME_SHUTDOWN();
    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}

