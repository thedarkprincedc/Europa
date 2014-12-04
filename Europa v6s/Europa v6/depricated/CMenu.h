#ifndef _CMENU_H_
#define _CMENU_H_
#include "../PHNX ENGINE/phoenixengine.h"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;

class CMenu{
	typedef void (*GAMESTATE)(void *, long);	 // Game State
	typedef map<const char*, CObject*>	TEXTINDEXMAP; // Icon Map
	typedef map<CObject*, GAMESTATE>	OBJECTMAP; // Icon Map
public:
	CMenu(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,
		  CStateManager *pStateManager = NULL);
	~CMenu();
	void SetMenuPosition(float x, float y);	// Sets the Menu Position
	void SetTextSize(float TextSize);			// Sets TextSize
	void Frame();							// Renders Text Frame
	void AddMenuItem(char *ItemName, GAMESTATE Function = NULL, 
		float x = 0.0f, float y = 0.0f);
	void AddIconItem(char *IconName, GAMESTATE Function = NULL, 
		float x = 0.0f, float y = 0.0f);
	void AddAnimatedIcon(char *IconName, GAMESTATE Function, 
		int numFrames, int frameWidth, int frameHeight, BOOL playOnce, 
		long timeIntervalMillisec, CAnimSprite::ANIMDIRECTION directionAnim,
		float x, float y);
	void deleteText(const char *pText);
	/* Mouse State Operations */
	CObject *CheckSelection(int x, int y);		// Checks Selection
	virtual void OnMouseOver(CObject *pFont);		// OnMouseOverEvent 
	virtual void OnMouseOut(CObject *pFont);		// OnMouseOutEvent
	virtual void OnMouseLeftClick(CObject *pObject);// OnMouseLClickEvent 
	virtual void OnMouseRightClick();			// OnMouseRClickEvent
	void TrackMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
	void AlignMenu();					  // AlignMenu - Performs Alignment 
										  //			 on all Items
	IDirect3DDevice9	*m_pDevice;		  // Direct3d Device
	ID3DXSprite			*m_pSprite;		  // Sprite Objects
	CStateManager		*m_pStateManager; // State Manager
	TEXTINDEXMAP		 m_IndexMap;
	OBJECTMAP		 	 m_ObjectMap;
	float				 m_MenuXPos, m_MenuYPos;
	float				 m_MouseXPos, m_MouseYPos;
};
class CMenuEx : public CMenu{
public:
	CMenuEx(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
		    CStateManager *pStateManager = NULL);
	void OnMouseOver(CObject *pObject);
	void OnMouseOut(CObject *pObject);
	BYTE OverAlpha;
};
#endif