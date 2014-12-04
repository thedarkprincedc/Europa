/************************************************
	Core Framework Classes
	Note: The classes take care of any 
		  initalization you need to do 
		  for direct3d and win32 and 
		  supports direct input although 
		  its kinda akward to use
		  When you include this header all of these 
		  classes are included may increase compile
		  times but havent looked into it. later ill
		  prolly include some preprocessor defines
	- BrettMosley
************************************************/
#ifndef _CORE_GLOBAL_H_
#define _CORE_GLOBAL_H_
/* Libraries */
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "dxguid.lib")
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "winmm.lib")
/* Defines  */
#define DIRECTINPUT_VERSION 0x0800
/* Windows Includes */
#include <vector>
#include <map>
#include <hash_map>
using namespace std;
using namespace stdext;
#include <windows.h>
/* DirectX Includes */
#include <d3d9.h>
#include <d3dx9.h>
#include <dinput.h>
/* Core Includes    */
#include "./Core_System.h"
#include "./Core_Graphics.h"
#include "./Core_Input.h"
#include "./Event_Manager.h"
#ifdef X360Control 
#include "./360Controller"
#endif 

#endif