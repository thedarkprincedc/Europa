#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "winmm.lib")
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "d3dutility.h"
#include "cdxcamera.h"
#include "cdxmesh.h"
//*************************************
// The Ground 
//#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)
typedef struct {
	FLOAT x, y, z;
	DWORD color;
	FLOAT u, v;
}CUSTOM_FVF;
IDirect3DVertexBuffer9	*m_pVertexBuffer;

void CreateGround(IDirect3DDevice9 *pDevice){
	pDevice->CreateVertexBuffer(4*sizeof(CUSTOM_FVF), 0, 
		PANEL_D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	CUSTOM_FVF *pVertices = NULL;
	m_pVertexBuffer->Lock(0, 4*sizeof(CUSTOM_FVF), (VOID**)&pVertices, 0);
	/*
	pVertices[0].y = pVertices[1].y = pVertices[2].y = pVertices[3].y = 1.0f;
	pVertices[0].x = -1;
	pVertices[0].z = -1;
	
	pVertices[1].x = -1;
	pVertices[1].z = 1;
	
	pVertices[2].x = 1;
	pVertices[2].z = -1;
	
	pVertices[3].x = 1;
	pVertices[3].z = 1;*/
	float m_fWidth = 300;
	float m_fHeight = 300;
	D3DCOLOR m_dwColor = D3DCOLOR_XRGB(255, 0,0);
	pVertices[0].x = -(m_fWidth) / 2.0f;
	pVertices[0].y = -(m_fHeight) / 2.0f;
	pVertices[0].z = 1.0f;
	pVertices[1].x = -(m_fWidth) / 2.0f;
	pVertices[1].y = (m_fHeight) / 2.0f;
	pVertices[1].z = 1.0f;
	pVertices[2].x = (m_fWidth) / 2.0f;
	pVertices[2].y = -(m_fHeight) / 2.0f;
	pVertices[2].z = 1.0f;
	pVertices[3].x = (m_fWidth) / 2.0f;
	pVertices[3].y = (m_fHeight) / 2.0f;
	pVertices[3].z = 1.0f;
	// Vertex Colors
	pVertices[0].color = m_dwColor;
	pVertices[1].color = m_dwColor;
	pVertices[2].color = m_dwColor;
	pVertices[3].color = m_dwColor;
	// Texture Coordinates
	pVertices[0].u = 0.0f;	// 1
	pVertices[0].v = 1.0f;	
	pVertices[1].u = 0.0f;	// 2
	pVertices[1].v = 0.0f;
	pVertices[2].u = 1.0f;  // 3
	pVertices[2].v = 1.0f;
	pVertices[3].u = 1.0f;	// 4
	pVertices[3].v = 0.0f;

	m_pVertexBuffer->Unlock();
}
void RenderGround(IDirect3DDevice9 *pDevice){
	D3DXMATRIX dm;
	D3DXMatrixIdentity(&dm);
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CUSTOM_FVF));
	pDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);
	D3DXMatrixRotationX(&dm, 0.02f);
	pDevice->SetTransform(D3DTS_WORLD, &dm);
	//		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

}




//*************************************
LPDIRECT3DDEVICE9 pDevice;
CMesh		*m_meshNew2;
CMesh       *m_meshNew;
CCamera		*m_Camera;
D3DXCOLOR lightColor = d3d::WHITE;
static void SetUp2DCamera(IDirect3DDevice9 *pD3DDevice, float m_ScreenWidth, float m_ScreenHieght){//Orthorgonal Project
	D3DXMATRIX matOrtho, matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	D3DXMatrixOrthoLH(&matOrtho, m_ScreenWidth, m_ScreenHieght, -250.0f, 1000.0f);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	pD3DDevice->SetTransform(D3DTS_VIEW, &matIdentity);
	//pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	//pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}
void GAME_INIT(){
	m_meshNew2 = new CMesh(pDevice, "./strider/lkj.x","./strider/");
	//m_meshNew = new CMesh(pDevice, "./maps/ship.x","./maps/");
	m_meshNew = new CMesh(pDevice, "./avion.X","./");
	//m_mesh->LoadMesh("./maps/ship.x","./maps/");
	D3DXMATRIX matProjection;
	D3DXMatrixPerspectiveFovLH(&matProjection, D3DX_PI/4, 1.3333f, 1.0f, 10000.0f);			
	pDevice->SetTransform(D3DTS_PROJECTION, &matProjection);
	//m_meshNew->RotateRel(0, D3DXToRadian(90), 0);
	//SetUp2DCamera(pDevice, 640, 480);
	D3DXVECTOR2 v = D3DXVECTOR2(0,0);
	float scalefactor = 0.5;
	m_meshNew->RotateRel(0,D3DXToRadian(90),D3DXToRadian(90));
	m_meshNew->SetScaleRel(-0.5, -0.5, -0.5);
	m_meshNew->CalculateBoundingColisionBox();
	float x = -((640/2) - ((m_meshNew->maxBounds.x*scalefactor - m_meshNew->minBounds.x*scalefactor)/2));
	float y = ((480/2) - ((m_meshNew->maxBounds.y*scalefactor - m_meshNew->minBounds.y*scalefactor)/2));
	
	
	//float x = -((640/2) - ((m_meshNew->maxBounds.x - m_meshNew->minBounds.x)/2));
	//float y = ((480/2) - ((m_meshNew->maxBounds.y - m_meshNew->minBounds.y)/2));
	//v.x = x;
	m_meshNew->SetTranslationAbs(x, y, 0);
	//D3DXMATRIX trans;
	//D3DXMatrixAffineTransformation2D(&trans, 0.5, NULL, 0, &v);
	//3DXMatrixTranslation(&trans, x, 0, 999);
	//pDevice->SetTransform(D3DTS_WORLD, &trans);
	
	m_Camera = new CCamera(AIRCRAFT);
//	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
//	pDevice->SetRenderState(D3DRS_AMBIENT, 0xFFFFFFFF);
 //pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );
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
	//D3DFILL_WIREFRAME : D3DFILL_SOLID
	CreateGround(pDevice);
}
bool GAME_LOOP(float frame){
	//D3DXMATRIX view;
	//pDevice->GetTransform(D3DTS_VIEW, &view);
	//m_Camera->GetViewMatrix(&view);
	//	if(GetAsyncKeyState(VK_UP))		m_Camera->Walk(+1);
	//	if(GetAsyncKeyState(VK_DOWN))	m_Camera->Walk(-1);
	//	if(GetAsyncKeyState(VK_LEFT))	m_Camera->Strafe(-1);
	//	if(GetAsyncKeyState(VK_RIGHT))	m_Camera->Strafe(+1);
	//pDevice->SetTransform(D3DTS_VIEW, &view);
	if(GetAsyncKeyState(VK_UP))		m_meshNew->SetTranslationRel(0,-1,0);
	if(GetAsyncKeyState(VK_DOWN))	m_meshNew->SetTranslationRel(0,1,0);
	if(GetAsyncKeyState(VK_LEFT))	m_meshNew->SetTranslationRel(-1,0,0);
	if(GetAsyncKeyState(VK_RIGHT))	m_meshNew->SetTranslationRel(1,0,0);
	
	pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);
	pDevice->BeginScene();
	//D3DXMATRIX trans;
	//::D3DXMatrixRotationY(&trans, y);
	//pDevice->SetTransform(D3DTS_WORLD, &trans);
	//m_mesh->Render(0.0f);
	RenderGround(pDevice);
	m_meshNew2->Render(pDevice);
	m_meshNew->Render(pDevice);
	pDevice->EndScene();
	pDevice->Present(NULL,NULL,NULL,NULL);
	return true;
}
void GAME_SHUTDOWN(){
}
LRESULT CALLBACK d3d::WndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch( msg ){
		case WM_CREATE:								break;
        case WM_DESTROY:    PostQuitMessage( 0 );   break;
		case WM_KEYDOWN:	if(wParam == VK_ESCAPE)
								PostQuitMessage( 0 );
			break;
    }
	return DefWindowProc( hwnd, msg, wParam, lParam );
}
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow){
	
	d3d::InitD3D(hInst, 640, 480, true, D3DDEVTYPE_HAL,&pDevice);
	GAME_INIT();
	d3d::EnterMsgLoop(GAME_LOOP);
	return 0;
}