#include "ScreenManager.h"
void SetUpLightsB(IDirect3DDevice9 *_pDevice){
	D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light;
	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = col * 0.4f;
	light.Diffuse   = col;
	light.Specular  = col * 0.6f;
	light.Direction = dir;
	_pDevice->SetLight(0, &light);
	_pDevice->LightEnable(0, true);
	_pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
}
void SetUp3DCameraB(IDirect3DDevice9 *pD3DDevice){
    // Set Projection Matrix
	D3DXMATRIX matProj, matView, matIdentity;
	D3DXMatrixIdentity(&matIdentity);
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.25f, 0.1f, 10000.0f);
	// Set View Matrix
    D3DXMatrixLookAtLH(&matView, 
		&D3DXVECTOR3(0.0f, 0.0f, -100.0f),		// Camera Position
        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// Look At Position
        &D3DXVECTOR3(0.0f, 1.0f, 0.0f)		// Up Direction
		);	
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj); 
    pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
}
ScreenManager *ScreenManager::instance = NULL;
ScreenManager *ScreenManager::getInstance()
{
	if(instance == NULL)	
		instance = new ScreenManager();
	return instance;
}
ScreenManager::ScreenManager(void)
{
}
ScreenManager::~ScreenManager(void)
{
}
void ScreenManager::Initialize(HWND &hWnd)
{
	graphics.Init();
	graphics.SetMode(hWnd, TRUE, TRUE, 800, 600, 32);
	SetUp3DCamera(graphics.GetD3DDevice());
	graphics.GetD3DDevice()->LightEnable(0, true);
	SetUpLightsB(graphics.GetD3DDevice());
	pGameObjectList.push_back(new SpaceShip(this));
	/*_pAvatar = new CAvatar();
	_pAvatar->load(graphics.GetD3DDevice(), "./maps/ship.x", "./maps");*/
}
void ScreenManager::Update()
{
	for each(DrawableGameObject *gameObject in pGameObjectList)
	{
		gameObject->Update();
	}
}
void ScreenManager::Draw()
{
	graphics.GetD3DDevice()->Clear(0,0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffff, 1.0f, 0);
	graphics.BeginScene();	
	for each(DrawableGameObject *gameObject in pGameObjectList)
	{
		gameObject->Draw();
	}
	graphics.EndScene();
	graphics.Display();
}
void ScreenManager::Shutdown()
{
}

