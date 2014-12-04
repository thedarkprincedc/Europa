#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
#include "./CSprite.h"

class CBackground	:	public CSpriteDx{
protected:
	typedef enum { NONE = 0, HORIZONTAL = 1, VERTICAL = 2, BOTH = 3} MOVEMENT;
	MOVEMENT	m_Direction;
	float		m_xOffset; 
	float		m_yOffset;
	float		m_xVelocity;
	float		m_yVelocity;
	D3DXVECTOR3 m_vPosition;
	RECT		m_rcRect;
public:

	CBackground(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,	
				char *pImgFilename, D3DCOLOR color = NULL);
	~CBackground();
	HRESULT Load(IDirect3DDevice9 *pDevice, char *pImgFilename, D3DCOLOR color = NULL){
		HRESULT r = CSpriteDx::Load(pDevice, pImgFilename, color);
		m_rcRect = CSpriteDx::GetRect();
		return r;
	}
	void setDirectionAbs(float x, float y);
	void Frame(IDirect3DDevice9* pDevice, ID3DXSprite *pSprite);
	void Update();
protected: 
	void Render(IDirect3DDevice9* pDevice, ID3DXSprite *pSprite);
};
#endif