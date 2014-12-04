#include <windows.h>
/*class SceneGraph{
public:
	CSprite *GetSpriteNode(char *pString);
protected:
	vector<SceneNode*> m_vSceneNode;
};
class SceneNode{	
	virtual HRESULT Render(IDirect3DDevice *pDevice) = 0;	
};
class SpriteNode : public SceneNode{
public:
	SpriteNode(IDirect3DDevice9 *pDevice, char *pTextureFilename, D3DCOLOR transColor = NULL);
	HRESULT Load(IDirect3DDevice9 *pDevice, char *pTextureFilename, D3DCOLOR transColor = NULL);
	HRESULT Render(IDirect3DDevice *pDevice);
protected:
	IDirect3DTexture9	*m_pTexture;
	D3DXVECTOR2			 m_vTranslate;
};
class CShipEntity{
	CShipEntity(SceneGraph *pSceneGraph);
	void Update();
}








class GameScene(){
	virtual void DrawScene();
	virtual void UpdateScene();
};


*/
void GAME_INIT(){}
void GAME_RENDER(){}
void GAME_SHUTDOWN(){}
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam ){
    switch( msg ){
		case WM_CREATE:								break;
        case WM_DESTROY:    PostQuitMessage( 0 );   break;
		case WM_KEYDOWN:							break;
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
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"Europa V6.3 beta www.rit.edu/btm6256 brettmosley@hotmail.com",
                              WS_OVERLAPPEDWINDOW, 100, 100, 1024, 768, NULL, NULL, wc.hInstance, NULL );
    // Show the window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );
    // Enter the message loop
    MSG msg;
    ZeroMemory( &msg, sizeof(msg) );
	GAME_INIT();
	while( msg.message != WM_QUIT ){
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) ){ // Active Program Loop
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		//else if(REGULATE_FRAMERATE()) 
			//GAME_LOOP();
    }
	GAME_SHUTDOWN();
    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}