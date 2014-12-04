/******************************************
	CXBoxController - A wrapper for xinput
******************************************/

#ifndef _X360CONTROLLER_H_
#define _X360CONTROLLER_H_
#pragma comment(lib, "xinput.lib")
#include <windows.h>
#include <commdlg.h>
#include <XInput.h> // XInput API
#include <basetsd.h>

struct CONTROLLER_STATE{
    XINPUT_STATE    state;       
    bool            bConnected;
};
class CXboxController{
public:
	CXboxController();
	const CONTROLLER_STATE *GetControllerState();
	void Update();
protected:
	CONTROLLER_STATE	m_Controllers;
};
#endif