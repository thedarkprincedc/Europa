/*******************************************
	CMeshPicker: Used for picking 3d Meshes  
				 casts a ray and picks the 
				 first mesh in its path
	BrettMosley@hotmail.com - April 13th 2006
********************************************/
#ifndef _CMESHPICKER_H_
#define _CMESHPICKER_H_
#include <d3d9.h>
#include <d3dx9.h>
#include "CXMesh.h"

class CMeshPicker{
public:
	CMeshPicker(IDirect3DDevice9 *pDevice);
	virtual ~CMeshPicker();
	void Frame();
	virtual void OnMouseOver(CXMesh *pMesh);
	virtual void OnLeftClick(CXMesh *pMesh);
	virtual void OnRightClick(CXMesh *pMesh);
	void TrackMouse(UINT uMsg, WPARAM wParam, LPARAM lParam);
	CXMesh *CheckSelection(int x, int y);
protected:
	typedef vector<CXMesh*> VECTOROFCPTRTOMESHES;
	BOOL _bLeft;
protected:
	struct MousePosition{
		float x_pos;
		float y_pos;
	};
	IDirect3DDevice9	   *m_pDevice;
	CXMesh					m_pCurrentMesh;
	VECTOROFCPTRTOMESHES	m_vCMesh;
	MousePosition			m_MousePos;
	float					ScreenWidth;
	float					ScreenHeight;
};
#endif