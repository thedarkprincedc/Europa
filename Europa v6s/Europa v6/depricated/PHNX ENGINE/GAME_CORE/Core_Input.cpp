#include "Core_Global.h"
/**********************************
	CInput
**********************************/
CInput::CInput(){ m_pDI = NULL; }
CInput::~CInput(){ Shutdown(); }
HWND CInput::GethWnd(){	return m_hWnd; }
IDirectInput8 *CInput::GetDirectInputCOM(){
	return m_pDI;
}
BOOL CInput::Init(HWND hWnd, HINSTANCE hInst){
	Shutdown();
	m_hWnd = hWnd;
	if(FAILED(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, 
								(void**)&m_pDI, NULL)))
		return FALSE;
	return TRUE;
}
BOOL CInput::Shutdown(){
	ReleaseCOM(m_pDI);
	m_hWnd = NULL;
	return TRUE;
}
/***************************************	
***************************************/
CInputDevice::CInputDevice(){
	m_Input			= NULL;
	m_Type			= NONE;
	m_Windowed		= TRUE;
	m_pDIDevice		= NULL;
	m_MouseState	= (DIMOUSESTATE*)&m_State;
	m_JoystickState = (DIJOYSTATE*)&m_State;
	Clear();
}
CInputDevice::~CInputDevice(){ Free(); }
IDirectInputDevice8 *CInputDevice::DeviceCOM(){
	return m_pDIDevice;
}
BOOL CInputDevice::Create(CInput *Input, short Type, BOOL Windowed){
	DIDATAFORMAT *DataFormat;
	DIPROPRANGE	 DIprg;
	DIPROPDWORD  DIpdw;
	Free();
	if((m_Input = Input) == NULL)
		return FALSE;
	switch(Type){
		case KEYBOARD:
			if(FAILED(m_Input->GetDirectInputCOM()->CreateDevice(GUID_SysKeyboard, 
				                                                 &m_pDIDevice, NULL)))
				return FALSE;
			DataFormat = (DIDATAFORMAT*)&c_dfDIKeyboard;
			break;
		case MOUSE:
			if(FAILED(m_Input->GetDirectInputCOM()->CreateDevice(GUID_SysMouse, 
																 &m_pDIDevice, NULL)))
				return FALSE;
			DataFormat = (DIDATAFORMAT*)&c_dfDIMouse;
			break;
		case JOYSTICK:
			if(FAILED(m_Input->GetDirectInputCOM()->EnumDevices(DI8DEVCLASS_GAMECTRL, 
																EnumJoysticks, this,
																DIEDFL_ATTACHEDONLY)))
				return FALSE;
			if(m_pDIDevice == NULL)
				return FALSE;
			DataFormat = (DIDATAFORMAT*)&c_dfDIJoystick;
			break;
		default:	
			return FALSE;
	}
	m_Windowed = Windowed;
	if(FAILED(m_pDIDevice->SetDataFormat(DataFormat)))
		return FALSE;
	if(FAILED(m_pDIDevice->SetCooperativeLevel(m_Input->GethWnd(), 
							DISCL_FOREGROUND|DISCL_NONEXCLUSIVE)))
		return FALSE;
	if(Type == JOYSTICK){
		DIprg.diph.dwSize = sizeof(DIPROPRANGE);
		DIprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		DIprg.diph.dwHow = DIPH_BYOFFSET;
		DIprg.lMin = -1024;
		DIprg.lMax = +1024;
		DIprg.diph.dwObj = DIJOFS_X;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_RANGE, &DIprg.diph)))
			return FALSE;
		DIprg.diph.dwObj = DIJOFS_Y;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_RANGE, &DIprg.diph)))
			return FALSE;
		DIpdw.diph.dwSize = sizeof(DIPROPDWORD);
		DIpdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		DIpdw.diph.dwHow = DIPH_BYOFFSET;
		DIpdw.dwData = 128;
		DIpdw.diph.dwObj = DIJOFS_X;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_DEADZONE, &DIpdw.diph)))
			return FALSE;
		DIpdw.diph.dwObj = DIJOFS_Y;
		if(FAILED(m_pDIDevice->SetProperty(DIPROP_DEADZONE, &DIpdw.diph)))
			return FALSE;
	}
	if(FAILED(m_pDIDevice->Acquire()))
		return FALSE;
	m_Type = Type;
	Clear();
	return FALSE;
}
BOOL CInputDevice::Free(){
	if(m_pDIDevice != NULL){
		m_pDIDevice->Unacquire();
		ReleaseCOM(m_pDIDevice);
	}
	m_Type = NONE;
	Clear();
	return TRUE;
}
BOOL CInputDevice::Clear(){
	short i;
	ZeroMemory(&m_State, 256);
	for(i = 0; i < 256; i++)
		m_Locks[i] = FALSE;
	m_XPos = 0;
	m_YPos = 0;
	return TRUE;
}
BOOL CInputDevice::Read(){
	HRESULT hr;
	long BufferSizes[3] = { 256, sizeof(DIMOUSESTATE), sizeof(DIJOYSTATE) };
	short i;
	if(m_pDIDevice == NULL)			return FALSE;
	if(m_Type < 1 || m_Type > 3)	return FALSE;
	
	while(1){
       m_pDIDevice->Poll();
	   if(SUCCEEDED(hr = m_pDIDevice->GetDeviceState(BufferSizes[m_Type - 1], (LPVOID)&m_State)))
		   break;
	   if(hr != DIERR_INPUTLOST && hr != DIERR_NOTACQUIRED)
		   return FALSE;
	   if(FAILED(m_pDIDevice->Acquire()))
		   return FALSE;
	}
	if(m_Type == MOUSE){
		if(m_Windowed == TRUE){
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(m_Input->GethWnd(), &pt);
			m_XPos = pt.x;
			m_YPos = pt.y;
		}
		else{
			m_XPos += m_MouseState->lX;
			m_YPos += m_MouseState->lY;
		}
	}
	switch(m_Type){
		case KEYBOARD:
			for(i = 0; i < 256; i++){
				if(!(m_State[i] & 0x80))
					m_Locks[i] = FALSE;
			}
		break;
		case MOUSE:
			for(i = 0; i < 4; i++){
				if(!(m_MouseState->rgbButtons[i]))
					m_Locks[i] = FALSE;
			}
		break;
		case JOYSTICK:
			for(i = 0; i < 32; i++){
				if(!(m_JoystickState->rgbButtons[i]))
					m_Locks[i] = FALSE;
			}
		break;
	}
	return TRUE;
}
BOOL CInputDevice::Acquire(BOOL bActive){
	if(m_pDIDevice == NULL)
		return FALSE;
	if(bActive == TRUE)
		m_pDIDevice->Acquire();
	else
		m_pDIDevice->Unacquire();
	return TRUE;
}
BOOL CInputDevice::GetLock(char Num){
	return m_Locks[Num];
}
BOOL CInputDevice::SetLock(char Num, BOOL State){
	m_Locks[Num] = State;
	return TRUE;
}
long CInputDevice::GetXPos(){
	if(m_Type == JOYSTICK)
		m_XPos = m_JoystickState->lX;
	return m_XPos;
}
BOOL CInputDevice::SetXPos(long XPos){
	m_XPos = XPos;
	return TRUE;
}
long CInputDevice::GetYPos(){
	if(m_Type == JOYSTICK)
		m_YPos = m_JoystickState->lY;
	return m_YPos;
}
BOOL CInputDevice::SetYPos(long YPos){
	m_YPos = YPos;
    return TRUE;
}
long CInputDevice::GetXDelta(){
	switch(m_Type){
		case MOUSE:
			return m_MouseState->lX;
		case JOYSTICK:
			return m_JoystickState->lX - m_XPos;
		default:	
			return 0;
	}
}
long CInputDevice::GetYDelta(){
	switch(m_Type){
		case MOUSE:
			return m_MouseState->lY;
		case JOYSTICK:
			return m_JoystickState->lY - m_YPos;
		default:	
			return 0;
	}
}
BOOL CInputDevice::GetKeyState(char Num){
	if(m_State[Num] & 0x80 && m_Locks[Num] == FALSE)
		return TRUE;
    return FALSE;
}
BOOL CInputDevice::SetKeyState(char Num, BOOL State){
	m_State[Num] = State;
	return TRUE;
}
BOOL CInputDevice::GetPureKeyState(char Num){
	return ((m_State[Num] & 0x80) ? TRUE : FALSE);
}
short CInputDevice::GetKeypress(long TimeOut){
	static HKL KeyboardLayout = GetKeyboardLayout(0);
	unsigned char WinKeyStates[256], DIKeyStates[256];
	unsigned short i, ScanCode, VirtualKey, Keys, Num;
	unsigned long EndTime;
	if((m_Type != KEYBOARD) || (m_pDIDevice == NULL))
		return 0;
	EndTime = GetTickCount() + TimeOut;
	while(1){
		GetKeyboardState(WinKeyStates);
		m_pDIDevice->GetDeviceState(256, DIKeyStates);
		for(i = 0; i < 256; i++){
			if(DIKeyStates[i] & 0x80){
				if((VirtualKey = MapVirtualKeyEx((ScanCode = i), 1, KeyboardLayout))){
					Num = ToAsciiEx(VirtualKey, ScanCode, WinKeyStates, &Keys, 0, KeyboardLayout);
					if(Num)
						return Keys;
				}
			}
		}
		if(TimeOut){
			if(GetTickCount() > EndTime)
				return 0;
		}
	}
	return 0;
}

long CInputDevice::GetNumKeypresses(){
	long i, Num = 0;
	for(i = 0; i < 256; i++){
		if(m_State[i] & 0x80 && m_Locks[i] == FALSE)
			Num++;
	}
	return Num;
}

long CInputDevice::GetNumPureKeypresses(){
	long i, Num = 0;
	for(i = 0; i < 256; i++){
		if(m_State[i] & 0x80)
			Num++;
	}
	return Num;
}

BOOL CInputDevice::GetButtonState(char Num){
	char State = 0;
	if(m_Type == MOUSE)
		State = m_MouseState->rgbButtons[Num];
	if(m_Type == JOYSTICK)
		State = m_JoystickState->rgbButtons[Num];
	if(State & 0x80 && m_Locks[Num] == FALSE)
		return TRUE;
	return FALSE;
}
BOOL CInputDevice::SetButtonState(char Num, BOOL State){
	if(m_Type == MOUSE){
		m_MouseState->rgbButtons[Num] = State;
		return TRUE;
	}
	if(m_Type == JOYSTICK){
		m_JoystickState->rgbButtons[Num] = State;
		return TRUE;
	}
	return FALSE;
}
BOOL CInputDevice::GetPureButtonState(char Num){
	if(m_Type == MOUSE)		return m_MouseState->rgbButtons[Num];
	if(m_Type == JOYSTICK)	return m_JoystickState->rgbButtons[Num];
	return FALSE;
}

long CInputDevice::GetNumButtonPresses(){
	long i, Num = 0;
	if(m_Type == MOUSE){
		for(i = 0; i < 4; i++){
			if(m_MouseState->rgbButtons[i] & 0x80 && m_Locks[i] == FALSE)
				Num++;
		}
	}
	else{
		if(m_Type == JOYSTICK){
			for(i = 0; i < 32; i++){
				if(m_JoystickState->rgbButtons[i] & 0x80 && m_Locks[i] == FALSE)
					Num++;
			}
		}
	}
	return Num;
}

long CInputDevice::GetNumPureButtonPresses(){
	long i, Num;
	if(m_Type == MOUSE){
		for(i = 0; i < 4; i++){
			if(m_MouseState->rgbButtons[i] & 0x80)
				Num++;
		}
	}
	else{
		if(m_Type == JOYSTICK){
			for(i = 0; i < 32; i++){
				if(m_JoystickState->rgbButtons[i] & 0x80)
					Num++;
			}
		}
	}
	return Num;
}

BOOL FAR PASCAL CInputDevice::EnumJoysticks(LPCDIDEVICEINSTANCE pdInst, LPVOID pvRef){
	CInputDevice *Input;
	if((Input = (CInputDevice*)pvRef) == NULL)
		return DIENUM_STOP;
	if(FAILED(Input->m_Input->GetDirectInputCOM()->CreateDevice(pdInst->guidInstance, 
				&Input->m_pDIDevice, NULL)))
		return DIENUM_CONTINUE;
	return DIENUM_STOP;
}