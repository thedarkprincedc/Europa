#ifndef _CORE_INPUT_H_
#define _CORE_INPUT_H_

#define ReleaseCOM(x) if(x){x->Release(); x = NULL;}

enum InputDevices{
	NONE = 0,
	KEYBOARD, 
	MOUSE,
	JOYSTICK
};
class CInput;
class CInputDevice;

class CInput{
protected:
	HWND				m_hWnd;
	IDirectInput8		*m_pDI;
public:
	CInput();
	~CInput();
	IDirectInput8 *GetDirectInputCOM();
	HWND GethWnd();
	BOOL Init(HWND hWnd, HINSTANCE hInst);
	BOOL Shutdown();
};

class CInputDevice{
public:
	CInput					*m_Input;
	short					m_Type;
	IDirectInputDevice8		*m_pDIDevice;
	BOOL					m_Windowed;
	char					m_State[256];
	DIMOUSESTATE			*m_MouseState;
	DIJOYSTATE				*m_JoystickState;
	BOOL					m_Locks[256];
	long					m_XPos, m_YPos;
	static BOOL FAR PASCAL EnumJoysticks(LPCDIDEVICEINSTANCE pdInst, LPVOID pvRef);
public:
	CInputDevice();
	~CInputDevice();
	IDirectInputDevice8 *DeviceCOM();
	BOOL Create(CInput *Input, short Type, BOOL Windowed = TRUE);
	BOOL Free();
	BOOL Clear();
	BOOL Read();
	BOOL Acquire(BOOL bActive = TRUE);
	BOOL GetLock(char Num);
	BOOL SetLock(char Num, BOOL State = TRUE);
	long GetXPos();
	BOOL SetXPos(long XPos);
	long GetYPos();
	BOOL SetYPos(long YPos);
	long GetXDelta();
	long GetYDelta();
	BOOL GetKeyState(char Num);
	BOOL SetKeyState(char Num, BOOL State);
	BOOL GetPureKeyState(char Num);
	short GetKeypress(long TimeOut = 0);
	long GetNumKeypresses();
	long GetNumPureKeypresses();
	BOOL GetButtonState(char Num);
	BOOL SetButtonState(char Num, BOOL State);
	BOOL GetPureButtonState(char Num);
	long GetNumButtonPresses();
	long GetNumPureButtonPresses();
};
#endif