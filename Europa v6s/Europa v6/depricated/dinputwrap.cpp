#include "./dinputwrap.h"
/*
void CKeyboard::Init(HWND hWnd){
	if(FAILED(DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&m_pDI, NULL))){
			return;
	}
	if(FAILED(m_pDI->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL))){
		return;
	}
	if(FAILED(m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard))){
		return;
	}
	if(FAILED(m_pKeyboardDevice->SetCooperativeLevel(hWnd, 
			DISCL_BACKGROUND | DISCL_NONEXCLUSIVE))){
				return;
	}
	if(FAILED(m_pKeyboardDevice->Acquire())){
		return;
	}
	if(FAILED(m_pDI->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL)))
		return;
	if(FAILED(m_pMouseDevice->SetCooperativeLevel(hWnd, 
				   DISCL_BACKGROUND | DISCL_NONEXCLUSIVE)))	return;
	if(FAILED(m_pMouseDevice->SetDataFormat(&c_dfDIMouse)))	return;
	if(FAILED(m_pMouseDevice->Acquire()))	return;
	POINT pt;
	GetCursorPos(&pt);
	m_bufferedX = pt.x;
}
BOOL CKeyboard::GetKeyState(int vkey){
	if(FAILED(m_pKeyboardDevice->GetDeviceState(sizeof(unsigned char[256]),
			 (LPVOID)m_Keystate))){
	}
	if(m_Keystate[vkey] & 0x80)
		return TRUE;
	return FALSE;
}

void CKeyboard::Poll(){	
	int k =0;
	k =55;
	if(FAILED(m_pKeyboardDevice->GetDeviceState(sizeof(unsigned char[256]),
			 (LPVOID)m_Keystate))){
	}
	if(m_Keystate[DIK_LCONTROL] & 0x80){
		::OutputDebugString("Wewgfvwegvwergvfwer\n");
	}
	if(FAILED(m_pMouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mousestate)))
		return;
}
short CKeyboard::GetKeyPress(long timeOut){	// GetAsciiKeyPress to be used with Typing
	static HKL KeyboardLayout = GetKeyboardLayout(0);
	unsigned char WinKeyStates[256], DIKeyStates[256];
	unsigned short i, ScanCode, VirtualKey, Keys, Num;
	unsigned long EndTime;
	EndTime = GetTickCount() + timeOut;
	while(1){
		GetKeyboardState(WinKeyStates);
		m_pKeyboardDevice->GetDeviceState(256, DIKeyStates);
		for(i = 0; i < 256; i++){
			if(DIKeyStates[i] & 0x80){
				if((VirtualKey = MapVirtualKeyEx((ScanCode = i), 1, KeyboardLayout))){
					Num = ToAsciiEx(VirtualKey, ScanCode, WinKeyStates, &Keys, 0, KeyboardLayout);
					if(Num)
						return Keys;
				}
			}
		}
		if(timeOut){
			if(GetTickCount() > EndTime)
				return 0;
		}
	}
	return 0;
}
int CKeyboard::GetXPos(){
	m_bufferedX += m_mousestate.lX;
	return m_mousestate.lX;
}
int CKeyboard::GetYPos(){
	return m_mousestate.lY;
}
DIMOUSESTATE *CKeyboard::GetMouseState(){
	return &m_mousestate;
}*/