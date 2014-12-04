#pragma once
#include "./Core_Graphics.h"
#include <d3d9.h>
#include <d3dx9.h>

#define getA(c) (((c)&0xff000000)>>24)
#define getR(c) (((c)&0x00ff0000)>>16)
#define getG(c) (((c)&0x0000ff00)>>8)
#define getB(c) ((c)&0x000000ff)

class CSpriteDx{
public:
	CSpriteDx(IDirect3DDevice9 *pDevice, char *pImgFilename, D3DCOLOR color = NULL){
		m_pTexture = GETCACHEDTEXTURE(pDevice, pImgFilename, color);
		if(m_pTexture != NULL){
			D3DSURFACE_DESC desc;				// Create Surface Description
			m_pTexture->GetLevelDesc(0, &desc);	// Get Surface Description
			m_Width = desc.Width;		// Get Width
			m_Height = desc.Height;		// Get Height
		}
		D3DXMatrixIdentity(&m_transform);
		D3DXMatrixIdentity(&m_scale);
		D3DXMatrixIdentity(&m_translate);
		D3DXMatrixIdentity(&m_rotation);
		ZeroMemory(&m_vRotationPoint,sizeof(m_vRotationPoint));
		ZeroMemory(&m_vScale,sizeof(m_vScale));
		ZeroMemory(&m_vTranslate,sizeof(m_vTranslate));
		m_ModulateColor = D3DCOLOR_XRGB(255,255,255);
		m_fRot = 0;
	}
	void Init();
	virtual HRESULT Load(IDirect3DDevice9 *pDevice, char *pImgFilename, D3DCOLOR color = NULL){
		m_pTexture = GETCACHEDTEXTURE(pDevice, pImgFilename, color);
		if(m_pTexture != NULL){
			D3DSURFACE_DESC desc;				// Create Surface Description
			m_pTexture->GetLevelDesc(0, &desc);	// Get Surface Description
			m_Width = desc.Width;		// Get Width
			m_Height = desc.Height;		// Get Height
			//*********************
			D3DXMatrixIdentity(&m_transform);
			D3DXMatrixIdentity(&m_scale);
			D3DXMatrixIdentity(&m_translate);
			D3DXMatrixIdentity(&m_rotation);
			ZeroMemory(&m_vRotationPoint,sizeof(m_vRotationPoint));
			ZeroMemory(&m_vScale,sizeof(m_vScale));
			ZeroMemory(&m_vTranslate,sizeof(m_vTranslate));
			m_ModulateColor = D3DCOLOR_XRGB(255,255,255);
			m_fRot = 0;	
			return S_OK;
		}
		return E_FAIL;
	}
	virtual void Render(IDirect3DDevice9* pDevice, ID3DXSprite *pSprite){
		m_transform = m_scale * m_rotation * m_translate;
		pSprite->SetTransform(&m_transform);
		pSprite->Draw(m_pTexture, NULL, NULL, NULL, m_ModulateColor);
		D3DXMatrixIdentity(&m_transform);
		pSprite->SetTransform(&m_transform);	
	}
	float GetWidth(){	return m_Width;		}
	float GetHeight(){	return m_Height;	}
	float GetXPos(){	return m_vTranslate.x;	}
	float GetYPos(){	return m_vTranslate.y;	}
	D3DXVECTOR2 GetPositionVector2() { return m_vTranslate; }
	RECT GetRect(){ 
		m_CollisionRect.left = m_vTranslate.x;
		m_CollisionRect.top = m_vTranslate.y;
		m_CollisionRect.right = (m_vTranslate.x+m_Width);
		m_CollisionRect.bottom = (m_vTranslate.y+m_Height);		
		return m_CollisionRect;
	}
	RECT m_CollisionRect;
	//Transforms 
	void SetTranslationAbs(float x, float y){
		m_vTranslate.x = x;		m_vTranslate.y = y;
		m_translate._41 = x;	m_translate._42 = y;
	}
	void SetTranslationRel(float x, float y){
		m_vTranslate.x += x;		m_vTranslate.y += y;
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
	float m_fRot;
	void SetModulateColorAbs(BYTE a, BYTE r, BYTE g, BYTE b){	m_ModulateColor = D3DCOLOR_ARGB(a,r,g,b);	}
	void SetModulateColorRel(BYTE a = 0, BYTE r = 0, BYTE g = 0, BYTE b = 0){
		D3DCOLOR color  = 0;
		color = D3DCOLOR_ARGB(a, 0, 0, 0);
		m_ModulateColor += color;
		color = D3DCOLOR_ARGB(0, r, 0, 0);
		m_ModulateColor += color;
		color = D3DCOLOR_ARGB(0, 0, g, 0);
		m_ModulateColor += color;
		color = D3DCOLOR_ARGB(0, 0, 0, b);
		m_ModulateColor += color;
	}
	void SetModulateColorAbs(D3DCOLOR modulateColor){	m_ModulateColor = modulateColor;	}
	D3DCOLOR GetModulateColor(){	return m_ModulateColor;	}
protected:
	IDirect3DTexture9	*m_pTexture;	 //	Texture of Entity
	float				m_Width;	 //	Texture Width
	float				m_Height;	//	Texture Height
	D3DXMATRIX			 m_transform;
	D3DXMATRIX			 m_scale;
	D3DXMATRIX			 m_translate;
	D3DXMATRIX			 m_rotation;
	D3DXVECTOR2			m_vRotationPoint;//	Rotation Point
	D3DXVECTOR2			m_vScale;		 //	Scale
	D3DXVECTOR2			m_vTranslate;	 //	Translation
	D3DCOLOR			m_ColorKey;		 //	Key Color for Transparency
	D3DCOLOR			m_ModulateColor; //	Moulate Color
	
	RECT				m_SourceRect;	 //
	//D3DXVECTOR3			m_SourcePosition;//
};
class CObjectTimer{
	long m_lastTime;
	long m_TimeInterval;	// Timing Settings
public:
	CObjectTimer();
	CObjectTimer(long i);
	void SetTime(long i);
	BOOL Run();
	BOOL RunOnce();
};

class CAnimSprite : public CSpriteDx{
public:
	typedef enum {NONE, FORWARD, REVERSE, OSCILLATE} ANIMDIRECTION;
protected:
	ANIMDIRECTION m_AnimDirection;
	// Animation Members
	int m_OffsetX, m_OffsetY;			// Offset 
	int m_nFrameWidth, m_nFrameHeight;  // Frame Dimensions
	int m_nNumOfFrames, m_nCurrFrame;   // Frame Counts
	BOOL m_bPlayOnce;					// Times to Play
	// Animation Timing
	CObjectTimer *m_Timer;
	int nextframe;
	inline void SetOffsets();
public:
	CAnimSprite(IDirect3DDevice9 *pDevice, char *pImgfilename, D3DCOLOR color = 0);
	CAnimSprite(IDirect3DDevice9 *pDevice, char *pImgfilename, int numFrames, int frameWidth, int frameHeight, 
		BOOL playOnce, long timeIntervalMillisec, ANIMDIRECTION directionAnim, D3DCOLOR color = 0);
	void SetActiveSprite(int frameNum);
	void SetupAnimation(int numFrames, int frameWidth, int frameHeight, 
		BOOL playOnce, long timeIntervalMillisec, ANIMDIRECTION directionAnim);
	void Forward();
	void Reverse();
	void Oscillate();
	void Render(IDirect3DDevice9* pDevice, ID3DXSprite *pSprite){
		switch(m_AnimDirection){
			case FORWARD:	Forward();	 break;
			case REVERSE:	Reverse();	 break;
			case OSCILLATE:	Oscillate(); break;
		}
		CSpriteDx::Render(pDevice, pSprite);
	}
};
