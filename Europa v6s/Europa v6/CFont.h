/****************************************************
 Brett Mosley - CFont Wrapper
 Wraps D3DX2D Font Class, and takes a sprite object 
		to speed up rendering
 DT_CALCRECT is used to calculation the rectangle 
		size of text
 Notes: This class only takes a pointer to a text string
		it does not allocate text string anymore because
		it was inflexable and not efficient
***************************************************/
#ifndef _CFONTEX_H_
#define _CFONTEX_H_
#include <d3d9.h>
#include <d3dx9.h>
#include "./Core_Graphics.h"
#define MAX_TEXT 255
class CFontEx{
public:
	CFontEx(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, char *fontname = "Arial", float fontsize = 12, D3DCOLOR color = D3DCOLOR_ARGB(255,255,255,255)){
		D3DXMatrixIdentity(&m_transform);
		D3DXMatrixIdentity(&m_scale);
		D3DXMatrixIdentity(&m_translate);
		D3DXMatrixIdentity(&m_rotation);
		ZeroMemory(m_vRotationPoint, sizeof(D3DXVECTOR2));
		ZeroMemory(m_vScale, sizeof(D3DXVECTOR2));
		ZeroMemory(m_vTranslate, sizeof(D3DXVECTOR2));
		m_fRot = 0;
		m_fontcolor = color;
		m_pFont = GETFONTOBJECT(pDevice, fontname, fontsize);
	}
	~CFontEx(){
	
	}
	RECT GetRect(ID3DXSprite *pSprite){
		ZeroMemory(&m_rcFont, sizeof(RECT));
		if(FAILED(m_pFont->DrawText(pSprite, wchTextString, 
			-1
			, &m_rcFont, DT_CALCRECT, m_fontcolor))){
			::OutputDebugString(L"FontSize(): FAILED");
		}
		m_Width = m_rcFont.right;
		m_Height = m_rcFont.bottom;

		m_rcFont.top	+= static_cast<LONG>(m_vTranslate.y);
		m_rcFont.left	+= static_cast<LONG>(m_vTranslate.x);
		m_rcFont.right	+= static_cast<LONG>(m_vTranslate.x);
		m_rcFont.bottom += static_cast<LONG>(m_vTranslate.y);
		return m_rcFont;
	}
	void SetText(char *pTextString){
		mbstowcs(wchTextString, pTextString, MAX_TEXT); // conversion
	}
	void SetColor(D3DCOLOR fontcolor){
		m_fontcolor = fontcolor;
	}
	void SetColorAbs(BYTE a, BYTE r, BYTE g, BYTE b){
		m_fontcolor = D3DCOLOR_ARGB(a, r, g, b);
	}
	void SetColorRel(BYTE a, BYTE r, BYTE g, BYTE b){
		m_fontcolor += D3DCOLOR_ARGB(a, r, g, b);
	}
	void SetTranslationAbs(float x, float y){
		m_vTranslate.x = x;		m_vTranslate.y = y;
		m_translate._41 = x;	m_translate._42 = y;
	}
	void SetTranslationRel(float x, float y){
		m_vTranslate.x += x;	m_vTranslate.y += y;
		m_translate._41 += x;	m_translate._42 += y;
	}
	void SetRotationAbs(float rotation){
		m_fRot = rotation;
		D3DXMatrixRotationZ(&m_rotation, m_fRot);
	}
	void SetRotationRel(float rotation){
		m_fRot += rotation;
		D3DXMatrixRotationZ(&m_rotation, m_fRot);
	}
	void SetRotationDegreeAbs(float rotationDegree){
		m_fRot = D3DXToDegree(rotationDegree);
		D3DXMatrixRotationZ(&m_rotation, m_fRot);
	}
	void SetRotationDegreeRel(float rotationDegree){
		m_fRot += D3DXToDegree(rotationDegree);
		D3DXMatrixRotationZ(&m_rotation, m_fRot);
	}
	void SetScaleAbs(float x, float y){
		m_vScale.x = x;		m_vScale.y = y;
		m_scale._11 = x;	m_scale._22 = y;
	}
	void SetScaleRel(float x, float y){
		m_vScale.x += x;		m_vScale.y += y;
		m_scale._11 += x;	m_scale._22 += y;
	}
	void Render(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
		m_transform = m_scale * m_rotation * m_translate;
		pSprite->SetTransform(&m_transform);
		m_pFont->DrawText(pSprite, wchTextString, -1, NULL, 0, m_fontcolor );
		D3DXMatrixIdentity(&m_transform);
		pSprite->SetTransform(&m_transform);
	}
protected:
	ID3DXFont			*m_pFont;		  // Font Object
	WCHAR				wchTextString[MAX_TEXT];   // Pointer TextString
	D3DXVECTOR2			m_vRotationPoint;//	Rotation Point
	D3DXVECTOR2			m_vScale;		 //	Scale
	D3DXVECTOR2			m_vTranslate;	 //	Translation
	
	float m_fRot;
	
	D3DXMATRIX			 m_transform;
	D3DXMATRIX			 m_scale;
	D3DXMATRIX			 m_translate;
	D3DXMATRIX			 m_rotation;

	RECT				m_rcFont;
	float				m_Width;
	float				m_Height;
	D3DCOLOR			m_fontcolor;
};

#endif