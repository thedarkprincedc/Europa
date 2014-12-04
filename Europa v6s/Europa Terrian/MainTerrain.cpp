#define _CRT_SECURE_NO_DEPRECATE
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
#include <windows.h>
#include "ScreenManager.h"
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
	{
		case WM_CREATE:								break;
        case WM_DESTROY:    PostQuitMessage( 0 );   break;
		case WM_KEYDOWN:							break;
    }
	return DefWindowProc( hWnd, msg, wParam, lParam );
}
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR, INT ){
    MSG msg;
    ZeroMemory( &msg, sizeof(msg) );
	// Register the window class
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"D3D Tutorial", NULL };
    RegisterClassEx( &wc );
    // Create the application's window
    HWND hWnd = CreateWindow( L"D3D Tutorial", L"Europa V6.3 beta www.rit.edu/btm6256 brettmosley@hotmail.com",
                              WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, wc.hInstance, NULL );
    // Show the window
    ShowWindow( hWnd, SW_SHOWDEFAULT );
    UpdateWindow( hWnd );
    // Enter the message loop
	ScreenManager *pScreenManager = ScreenManager::getInstance();
	pScreenManager->Initialize(hWnd);
	while( msg.message != WM_QUIT )
	{
        if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
		{	// Active Program Loop
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
		pScreenManager->Update();
		pScreenManager->Draw();
    }
	pScreenManager->Shutdown();
    UnregisterClass( L"D3D Tutorial", wc.hInstance );
    return 0;
}