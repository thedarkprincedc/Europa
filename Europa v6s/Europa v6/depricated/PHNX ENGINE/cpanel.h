/*****************************************************
	CPanel Class - that can be used to replace the 
				   current d3dx sprite rendering method
				   uses the flexable vertex pipeline.
				   I tested it by rendering a life bar
				   just need to implement it.
				   I found it at www.c-unit.com
*****************************************************/
#ifndef _CPANELEX_H_
#define _CPANELEX_H_
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
/*#include "./CWorldTransform.h"
#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
class CPanelEx : public CWorldTransform{
	typedef struct {
	FLOAT x, y, z;
	DWORD color;
	FLOAT u, v;
	}PANEL_CUSTOMVERTEX;
public:
	CPanelEx(IDirect3DDevice9 *pDevice, float width, float height,	
		float ScreenWidth, float ScreenHeight, DWORD dwColor);
	CPanel(IDirect3DDevice9 *pDevice, IDirect3DTexture9	*pTexture,
		int ScreenWidth, int ScreenHeight);
	virtual ~CPanelEx();
	BOOL SetTexture(char *pszFilename, DWORD dwKeyColor = 0);
	virtual void MoveTo(float x, float y);
	void Render();
	float getWidth();
	float getHeight();
	float getX();
	float getY();
protected:
	BOOL CreateVertexBuffer();	// Create VertexBuffer
	BOOL UpdateVertices();		// Creates Vertices
	IDirect3DDevice9		*m_pDevice;			// Device
	IDirect3DTexture9		*m_pTexture;		// Texture 
	IDirect3DVertexBuffer9	*m_pVertexBuffer;	// VertexBuffer
	DWORD					 m_dwColor;	
	float					 m_nWidth,		 m_nHeight;
	float					 m_nScreenWidth, m_nScreenHeight;		
	float					 m_x, m_y;
	//CWorldTransform			 m_WorldTransform;
};*/
#endif