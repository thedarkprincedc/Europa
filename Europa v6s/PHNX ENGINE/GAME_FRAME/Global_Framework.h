/********************************************************
	Global Framework - Includes classes	that take care of
		everyday things	such as sprite, fonts
	Classes: 
	CObject: Derive different Objects that have renderable 
		propeties from here such as Sprites Meshes and Fonts and 
		you can  perform common functions on them
	CSound: Uses directshow filter graph to play mp3s and wmas 
		with low latency
	CFonts: Fonts takes care of font rendering uses d3dx library 
		so not compatible with machines that dont have the dll 
		it was compiles with so you need send the necessary dll 
		with your executable
	CSpriteManager: Singlton Class that makes sure that one intance 
		of an renderable image is rendered used with in CSprite
	CBackground: Uses CSprite in order to render scrolling backgrounds 
	CMenu: Renders clickable menus (functional needs some work)
BrettMosley@hotmail.com - April 13th 2006
*********************************************************/
#ifndef _FRAMEWORK_GLOBAL_H_
#define _FRAMEWORK_GLOBAL_H_
#include "../GAME_CORE/Core_Global.h"
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
// Global Framework
#include "./CWorldTransform.h"
#include "./CPanel.h"
#include "./CSpriteManager.h"
#include "./CRenderObject.h"
#include "./CSprite.h"
#include "./CFont.h"
#include "./CSound.h"
#include "./CBackground.h"
#endif