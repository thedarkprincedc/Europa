#include "Core_Global.h"
CApplication::CApplication(){
	g_Application = this;
	m_hWnd = NULL;
	m_hInst = GetModuleHandle(NULL);
	//m_Class = "";
	strcpy(m_Class, "App");
	strcpy(m_Caption, "App");
	//m_Caption = "";
	m_Style = WS_OVERLAPPEDWINDOW;
	m_XPos = 0;
	m_YPos = 0;
	m_Width = 256;
	m_Height = 256;
	m_wcex.cbSize = sizeof(WNDCLASSEX);
	m_wcex.style = CS_CLASSDC;
	m_wcex.lpfnWndProc = AppWindowProc;
	m_wcex.cbClsExtra = 0;
	m_wcex.cbWndExtra = 0;
	m_wcex.hInstance = m_hInst;
	m_wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	m_wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	m_wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_wcex.hbrBackground = NULL;
	m_wcex.lpszMenuName = NULL;
	WCHAR wchClass[255];
	mbstowcs(wchClass, m_Class, 255);
	m_wcex.lpszClassName = wchClass;
}
HWND CApplication::GethWnd(){
	return m_hWnd;
}
HINSTANCE CApplication::GethInst(){
	return m_hInst;
}
BOOL CApplication::Run(){
	MSG msg;
	if(!RegisterClassEx(&m_wcex))
		return FALSE;
	WCHAR wchClass[255];
	WCHAR wchCaption[255];
	mbstowcs(wchClass, m_Class, 255);
	mbstowcs(wchCaption, m_Caption, 255);
	m_hWnd = CreateWindow(wchClass, wchCaption, 
		m_Style, m_XPos, m_YPos,
		m_Width, m_Height, 
		NULL, NULL, m_hInst, NULL);
	//m_hWnd = CreateWindow(L"tyjthjntf", L"thfgh", 
	//	m_Style, m_XPos, m_YPos,
	//	m_Width, m_Height, 
	//	NULL, NULL, m_hInst, NULL);
	
	if(!m_hWnd)
		return FALSE;
	ShowWindow(m_hWnd, SW_NORMAL);
	UpdateWindow(m_hWnd);
	Resize(m_Width, m_Height);
	CoInitialize(NULL);
	if(Init() == TRUE){
		ZeroMemory(&msg, sizeof(MSG));
		while(msg.message != WM_QUIT){
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else{
				if(Frame() == FALSE)
					break;
			}
		}
	}
	Shutdown();
	CoUninitialize();
	UnregisterClass(wchClass, m_hInst);
	return TRUE;
}
BOOL CApplication::Error(BOOL fatal, char *text){
	return TRUE;
}
BOOL CApplication::Move(long XPos, long YPos){
	RECT ClientRect;
	GetClientRect(m_hWnd, &ClientRect);
	MoveWindow(m_hWnd, XPos, YPos, ClientRect.right, ClientRect.bottom, TRUE);
	return TRUE;
}
BOOL CApplication::Resize(long width, long height){
	RECT WndRect, ClientRect;
	long WndWidth, WndHeight;
	GetWindowRect(m_hWnd, &WndRect);
	GetClientRect(m_hWnd, &ClientRect);
	WndWidth = (WndRect.right - (ClientRect.right - width)) - WndRect.left;
	WndHeight = (WndRect.bottom - (ClientRect.bottom - height)) - WndRect.top;
	MoveWindow(m_hWnd, WndRect.left, WndRect.top, WndWidth, WndHeight, TRUE);
	return TRUE;
}
BOOL CApplication::ShowMouse(BOOL Show){
	ShowCursor(Show);
	return TRUE;
}
static long FAR PASCAL AppWindowProc(HWND hWnd, UINT uMsg, 
									 WPARAM wParam, LPARAM lParam){
	switch(uMsg){
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		default:
			return g_Application->MsgProc(hWnd, uMsg, wParam, lParam);
	}
}
CStateManager::CStateManager() : m_StateParent(NULL){}
CStateManager::~CStateManager(){ PopAll(); }
void CStateManager::Push(void (*Function)(void *Ptr, long Purpose), void *DataPtr){
	sState *StatePtr;
	if(Function != NULL){
		StatePtr = new sState();
		StatePtr->Function = Function;
		StatePtr->Next = m_StateParent;
		m_StateParent = StatePtr;
		StatePtr->Function(DataPtr, INITPURPOSE);
	}
}
BOOL CStateManager::Pop(void *DataPtr){
	sState *StatePtr;
	if((StatePtr = m_StateParent) != NULL){
		m_StateParent->Function(DataPtr, SHUTDOWNPURPOSE);
		m_StateParent = StatePtr->Next;
		StatePtr->Next = NULL;
		delete StatePtr;
	}
	if(m_StateParent == NULL)
		return FALSE;
	return TRUE;
}
void CStateManager::PopAll(void *DataPtr){
	while(Pop(DataPtr) == TRUE);
}
BOOL CStateManager::Process(void *DataPtr){
	if(m_StateParent == NULL)
		return FALSE;
	m_StateParent->Function(DataPtr, FRAMEPURPOSE);
	return TRUE;
}
INT64 CFrameRate::m_Frequency = 0;
INT64 CFrameRate::m_FrameCount = 0;
INT64 CFrameRate::m_FrameRate = 0;
INT64 CFrameRate::NewCount = 0;	
INT64 CFrameRate::LastCount = 0;
CFrameRate *CFrameRate::m_CFrameRateSingleton = 0;