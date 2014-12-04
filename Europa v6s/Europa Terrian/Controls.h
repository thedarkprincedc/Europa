#pragma once
static bool bLeft = false;
static bool bRight = false;
static bool bUp = false;
static bool bDown = false;
static void UpdateControls()
{
	if(GetAsyncKeyState(VK_LEFT))	
		bLeft = true;
	else	
		bLeft = false;
	if(GetAsyncKeyState(VK_RIGHT))	
		bRight = true;
	else	
		bRight = false;
	if(GetAsyncKeyState(VK_UP))
		bUp = true;
	else	
		bUp = false;
	if(GetAsyncKeyState(VK_DOWN))	
		bDown = true;
	else
		bDown = false;
}