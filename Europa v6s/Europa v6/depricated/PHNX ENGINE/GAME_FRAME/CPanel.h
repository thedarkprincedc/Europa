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
#include "./Global_Framework.h"
#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)

class CPanelEx : public CWorldObject{
	typedef struct {
	FLOAT x, y, z;
	DWORD color;
	FLOAT u, v;
	}PANEL_CUSTOMVERTEX;
public:
	CPanelEx(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *pTexture);
	CPanelEx(IDirect3DDevice9 *pDevice, char *pszFilename, 
		D3DCOLOR KeyColor = NULL, float x = 0, float y = 0);
	CPanelEx(IDirect3DDevice9 *pDevice, float width, float height, 
		DWORD KeyColor = NULL);
	CPanelEx(CPanelEx &a);
	IDirect3DTexture9 *GetTexture();
	IDirect3DVertexBuffer9 *GetVertexBuffer();
	virtual ~CPanelEx();
	BOOL SetTexture(IDirect3DDevice9 *pDevice, char *pszFilename, DWORD dwKeyColor = 0);
	void TranslateAbs(float x, float y);
	void TranslateRel(float x, float y);
	void Render(IDirect3DDevice9 *pDevice);
	float GetWidth();
	float GetHeight();
	float GetX();
	float GetY();
	float GetScreenWidth();
	float GetScreenHeight();
	void AutoCenter(){
		TranslateAbs((m_nScreenWidth - m_fWidth)/2,0);
	}
protected:
	BOOL CreateVertexBuffer(IDirect3DDevice9 *pDevice);	// Create VertexBuffer
	BOOL UpdateVertices();		// Creates Vertices
	IDirect3DTexture9		*m_pTexture;		// Texture 
	IDirect3DVertexBuffer9	*m_pVertexBuffer;	// VertexBuffer
	float					 m_fWidth;
	float					 m_fHeight;
	float					 m_nScreenWidth;
	float					 m_nScreenHeight;
	float					 m_fx, m_fy;
	float					 m_fRealY;
	DWORD					 m_dwColor;	
};
#endif