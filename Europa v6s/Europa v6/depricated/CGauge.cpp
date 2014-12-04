#include "../PHNX_CORE/Core_Global.h"
#include "cdxmesh.h"

class TestGame : public CApplication{
private:
	CGraphics	m_CGraphics;
	CDXMesh		*m_mesh;
	CCamera		*m_pFlexCamera;
public:
	TestGame(){
		m_Width  = 640; 
		m_Height = 480;
		m_Style  = WS_BORDER | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
		strcpy(m_Class, "TestGameClass");
		strcpy(m_Caption, "Test Game Class");
	}
	BOOL Init(){
		m_CGraphics.Init();
		m_CGraphics.SetMode(GethWnd(), TRUE, TRUE, 640, 480, 16);
		//m_CGraphics.SetMode(GethWnd(), FALSE, FALSE);
		m_CGraphics.EnableAlphaBlending(FALSE);
		m_CGraphics.EnableAlphaTesting(FALSE);
		m_CGraphics.SetPerspective(D3DX_PI/4, 1.3333f, 1.0f, 10000.0f);
	
		m_mesh = new CDXMesh(m_CGraphics.GetD3DDevice());
		m_mesh->LoadMesh("./strider/lkj.x","./strider/");
		m_pFlexCamera = new CCamera();
		
		return TRUE;
	}
	BOOL Shutdown(){
		return TRUE;
	}
	void CameraControls(IDirect3DDevice9 *pDevice){
		D3DXMATRIX view;
		pDevice->GetTransform(D3DTS_VIEW, &view);
		m_pFlexCamera->GetViewMatrix(&view);
		if(GetAsyncKeyState(VK_UP))		m_pFlexCamera->Walk(+1);
		if(GetAsyncKeyState(VK_DOWN))	m_pFlexCamera->Walk(-1);
		if(GetAsyncKeyState(VK_LEFT))	m_pFlexCamera->Strafe(-1);
		if(GetAsyncKeyState(VK_RIGHT))	m_pFlexCamera->Strafe(+1);
		if(GetAsyncKeyState(0x005A))	m_pFlexCamera->Fly(+1);
		if(GetAsyncKeyState(0x0058))	m_pFlexCamera->Fly(-1);
		if(GetAsyncKeyState(0x0041))	m_pFlexCamera->Pitch(+.15);
		if(GetAsyncKeyState(0x0053))	m_pFlexCamera->Pitch(-.15);
		if(GetAsyncKeyState(0x0051))	m_pFlexCamera->Roll(-.15);
		if(GetAsyncKeyState(0x0057))	m_pFlexCamera->Roll(+.15);
		if(GetAsyncKeyState(0x0045))	m_pFlexCamera->Yaw(-.15);
		if(GetAsyncKeyState(0x0052))	m_pFlexCamera->Yaw(+.15);
		pDevice->SetTransform(D3DTS_VIEW, &view);
	}
	BOOL Frame(){
		m_CGraphics.Clear(0xffffffff);
		m_CGraphics.BeginScene();
		CameraControls(m_CGraphics.GetD3DDevice());
		m_mesh->Render(30);
		m_CGraphics.EndScene();
		m_CGraphics.BeginSprite();
			
		m_CGraphics.EndSprite();
		m_CGraphics.Display();
		return TRUE;
	}
	int FAR PASCAL MsgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{	
		switch(uMsg){
			case WM_KEYDOWN:{
				switch(wParam)
				{	case VK_SPACE:
					PostQuitMessage(0);
				}
				return 0;
			}
		}
		return (int)DefWindowProc(hWnd, uMsg, wParam, lParam);	
	}
};
int PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR szCmdLine, int nCmdShow){
  TestGame TestGame;
  return TestGame.Run();
}