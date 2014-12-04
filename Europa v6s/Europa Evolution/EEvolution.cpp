#define _CRT_SECURE_NO_DEPRECATE
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#include "../Europa v6/Core_Graphics.h"
#include "../Europa v6/CMesh.h"
#include "../Europa v6/CStatemanager.h"
#include "../Europa v6/CBackground.h"
#include "./cemitter.h"
#include <windows.h>
class D3DXGroupNode : public GroupNode{
public:
	virtual void Activate(IDirect3DDevice9 *pDevice) = 0;
	virtual void Render(IDirect3DDevice9 *pDevice) = 0;
	virtual void DeActivate(IDirect3DDevice9 *pDevice) = 0;
};
class D3DXSceneGraph : public SceneGraph{
public:
	void Render(IDirect3DDevice9 *pDevice){
		D3DXGroupNode *pD3DX = NULL;
		for(int i = 0; i < m_vpGroupNodes.size(); i++){
			pD3DX = (D3DXGroupNode*)m_vpGroupNodes[i];
			pD3DX->Activate(pDevice);
			pD3DX->Render(pDevice);
			pD3DX->DeActivate(pDevice);
		}
	}
};
class MeshObjectGroup : public D3DXGroupNode{
public:
	void AddMeshObject(CMesh *pMesh){
		m_vCMeshObject.push_back(pMesh);
	}
	void Render(IDirect3DDevice9 *pDevice){
		for(int i = 0; i < m_vCMeshObject.size(); i++)
			m_vCMeshObject[i]->Render(pDevice);
	}
	void Activate(IDirect3DDevice9 *pDevice){
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	}
	void DeActivate(IDirect3DDevice9 *pDevice){
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	}
	vector<CMesh*> m_vCMeshObject;
};
class SpriteObjectGroup : public D3DXGroupNode{  
public:
	SpriteObjectGroup(ID3DXSprite *pSprite){
		m_pSprite = pSprite;
	}
	void AddSpriteObject(CBackground *pSpriteDx){
		if(pSpriteDx != NULL)
			m_vCSpriteObject.push_back(pSpriteDx);
	}
	void Render(IDirect3DDevice9 *pDevice){
		for(int i = 0; i < m_vCSpriteObject.size(); i++)
			m_vCSpriteObject[i]->Render(pDevice, m_pSprite);
	}
	void Activate(IDirect3DDevice9 *pDevice){
		pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		m_pSprite->Begin(0);
	}
	void DeActivate(IDirect3DDevice9 *pDevice){
		m_pSprite->End();
	}
	ID3DXSprite *m_pSprite;
	vector<CSpriteDx*> m_vCSpriteObject;
};
class ParticleObjectGroup : public D3DXGroupNode{  
public:
	ParticleObjectGroup(){
		
	}
	void AddParticleObject(CEmitter *pEmitter){
		if(pEmitter != NULL)
			m_pEmitter = pEmitter;
	}
	void Render(IDirect3DDevice9 *pDevice){
		m_pEmitter->Render(pDevice);
	}
	void Activate(IDirect3DDevice9 *pDevice){
		SetUp3DCamera(pDevice);
		D3DXMATRIX ViewMatrix, RotMatrix;
		D3DXMatrixRotationY(&RotMatrix, timeGetTime()/1300.0f);
		D3DXVECTOR3 vecEye(0.0f, 10.0f, -10.0f);
		D3DXVec3TransformCoord(&vecEye, &vecEye, &RotMatrix); 
		D3DXMatrixLookAtLH(&ViewMatrix, &vecEye, &D3DXVECTOR3(0.0f, 0.0f, 0.0f), &D3DXVECTOR3(0.0f,1.0f,0.0f));
		pDevice->SetTransform(D3DTS_VIEW, &ViewMatrix);

		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}
	void DeActivate(IDirect3DDevice9 *pDevice){
		SetUp2DCamera(pDevice, 1024, 768);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	CEmitter *m_pEmitter;
};
#define TITLEBKG "./atmosphere.jpg"
CGraphics			*_pGraphics = new CGraphics();
CStateManager		*_pStateManager = new CStateManager();
CBackground  *_pBackground;
CEmitter			*pEmitter;
D3DXSceneGraph		*pSceneGraph = new D3DXSceneGraph();
MeshObjectGroup		*pMeshObjectGroup = new MeshObjectGroup(); 
SpriteObjectGroup	*pSpriteObjectGroup;
ParticleObjectGroup *pParticleGroup = new ParticleObjectGroup();
class CraftAvatar : public CMesh{
public:
	CraftAvatar(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath = "./") : CMesh(pDevice, pszFilename, pszTexturepath){
	//	m_MatrixStack->RotateYawPitchRollLocal(D3DXToRadian(-90),D3DXToRadian(-90),0);
	//	m_MatrixStack->ScaleLocal(0.3f,0.3f,0.3f);
	}
};
static void SetUpLights(){
	IDirect3DDevice9 *pDevice = _pGraphics->GetD3DDevice();
	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light;
	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = col * 0.4f;
	light.Diffuse   = col;
	light.Specular  = col * 0.6f;
	light.Direction = dir;
	pDevice->SetLight(0, &light);
	pDevice->LightEnable(0, true);
	//pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE );
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//pDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
	pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}
#define PLAYER_XSPEED 2
#define PLAYER_YSPEED 5
void PLAYER_PLANECONTROLS(CMesh *pMesh){
	//D3DXVECTOR3 vecRot = pMesh->GetRotation();
	//pEmitter->SetOrigin(pMesh->GetTranslation());
	//if(::GetAsyncKeyState(VK_UP)){
	//	if(vecRot.x < D3DXToRadian(24))
	//		pMesh->SetRotateRel(D3DXToRadian(1), 0, 0);
	//	pMesh->SetTranslationRel(0, 1 * PLAYER_YSPEED, 0);
	//}
	//else if(::GetAsyncKeyState(VK_DOWN)){
	//	if(vecRot.x > -D3DXToRadian(24))
	//		pMesh->SetRotateRel(D3DXToRadian(-1), 0, 0);
	//	pMesh->SetTranslationRel(0, -1 * PLAYER_YSPEED, 0);
	//}
	//else{
	//	if(vecRot.x < 0)
	//		pMesh->SetRotateRel(D3DXToRadian(1), 0, 0);
	////	else if(vecRot.x > 0)
	////		pMesh->SetRotateRel(D3DXToRadian(-1), 0, 0);
	//}
	//if(GetAsyncKeyState(VK_LEFT))	pMesh->SetTranslationRel(-5, 0, 0);
	//if(GetAsyncKeyState(VK_RIGHT))	pMesh->SetTranslationRel(5, 0, 0);
}
void GAME_STATE(void *Ptr, long Purpose){
	//static CMesh *mesh = new CMesh(_pGraphics->GetD3DDevice(), "./avion.x", "./");
	static CMesh *mesh = new CraftAvatar(_pGraphics->GetD3DDevice(), "./maps/ship.X", "./maps");
	switch(Purpose){
		case INITPURPOSE:
			//mesh->SetRotateRel(D3DXToRadian(-90), D3DXToRadian(-90), 0);
			SetUp2DCamera(_pGraphics->GetD3DDevice(), 1024, 768);
			//::SetUp3DCamera(_pGraphics->GetD3DDevice());
			//SetUpLights();
			_pBackground = new CBackground(_pGraphics->GetD3DDevice(), _pGraphics->GetSpriteObject(), TITLEBKG);
			_pBackground->setDirectionAbs(-3, 0);
			pMeshObjectGroup->AddMeshObject(mesh);
			pSpriteObjectGroup->AddSpriteObject(_pBackground);
			return;
		case SHUTDOWNPURPOSE:
			return;
		case FRAMEPURPOSE:
			pSceneGraph->Render(_pGraphics->GetD3DDevice());
			//pEmitter->Render(_pGraphics->GetD3DDevice());
			/*mesh->SetRotateRel(4,0,0);*/
			mesh->Render(_pGraphics->GetD3DDevice());
			PLAYER_PLANECONTROLS(mesh);
			return; 
	}
}

void GAME_INIT(HWND &hWnd){
	_pGraphics->Init();
	_pGraphics->SetMode(hWnd, TRUE, TRUE, 1024, 768, 32);
	pEmitter = new CParticleExplosion(_pGraphics->GetD3DDevice());
	pSpriteObjectGroup = new SpriteObjectGroup(_pGraphics->GetSpriteObject());
	//pSceneGraph->AttachGroupNode(pSpriteObjectGroup);
	pSceneGraph->AttachGroupNode(pMeshObjectGroup);
	pSceneGraph->AttachGroupNode(pParticleGroup);
	pParticleGroup->AddParticleObject(pEmitter);
	_pStateManager->Push(GAME_STATE);
}
void GAME_LOOP(){
	//_pGraphics->Clear(0xffffffff);	//	Clear Display
//	_pGraphics->GetD3DDevice()->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffff, 1.0f, 0);
//_pGraphics->GetD3DDevice()->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,55), 1.0f, 0);
	_pGraphics->GetD3DDevice()->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);
	_pGraphics->BeginScene();		//	Begin Scene
	_pStateManager->Process();
	_pGraphics->EndScene();			//	End Scene
	_pGraphics->Display();			//	Display Back Buffer
}
void GAME_SHUTDOWN(){
}
void GAME_RESET(){
}
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ){
    switch( msg ){
		case WM_CREATE:								break;
        case WM_DESTROY:    PostQuitMessage( 0 );  	break;
		case WM_KEYDOWN:	break;
    }
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT ){
    // Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"D3D Tutorial", NULL };
    RegisterClassEx( &wc );
    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"Europa Evolution beta www.rit.edu/btm6256 brettmosley@hotmail.com",
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