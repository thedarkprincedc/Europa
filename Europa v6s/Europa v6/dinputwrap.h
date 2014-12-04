#ifndef _DINPUTWRAP_H_
#define _DINPUTWRAP_H_
#include <dinput.h>
class CKeyboard{
public:
	CKeyboard(){}
	~CKeyboard(){}
	void Init(HWND hWnd);
	void Poll();
	BOOL GetKeyState(int vkey);			// Gets the State of key
	short GetKeyPress(long timeOut);	// GetAsciiKeyPress to be used with Typing
	int GetXPos();
	int GetYPos();
	DIMOUSESTATE *GetMouseState();
protected:
	LPDIRECTINPUT		m_pDI;
	LPDIRECTINPUTDEVICE m_pKeyboardDevice;
	unsigned char		m_Keystate[256];	// Holds Keyboard State Information
	LPDIRECTINPUTDEVICE m_pMouseDevice;
	DIMOUSESTATE		m_mousestate;
	int					m_bufferedX;
};
#endif