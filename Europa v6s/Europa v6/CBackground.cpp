#include "./CBackground.h"

CBackground::CBackground(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
						 char *pImgFilename, D3DCOLOR color)
		:	CSpriteDx(pDevice, pImgFilename, color){
	m_Direction = NONE;
	m_vPosition = D3DXVECTOR3(0,0,0);
	m_xOffset = m_yOffset = 0;
	m_xVelocity = 0;
	m_yVelocity = 0;
	m_rcRect.left = m_rcRect.top = 0;
	m_rcRect.right = static_cast<long>(GetWidth());
	m_rcRect.bottom = static_cast<long>(GetHeight());
	D3DXMatrixTranslation(&m_translate, 0,0,1);
}
CBackground::~CBackground(){}

void CBackground::setDirectionAbs(float x, float y){
	m_xVelocity = x;
	m_yVelocity = y;
}
void CBackground::Render(IDirect3DDevice9* pDevice, ID3DXSprite *pSprite){
	Update();
	m_xOffset = (m_vPosition.x < 0) ? GetWidth() : -GetWidth();
	m_vPosition.x =(m_vPosition.x < -GetWidth() || m_vPosition.x > GetWidth()) ? 0 : m_vPosition.x;
	m_yOffset=(m_vPosition.y<0)?GetHeight():-GetHeight();
	m_vPosition.y =(m_vPosition.y < -GetHeight()||m_vPosition.y > GetHeight()) ? 0 : m_vPosition.y;	
	D3DXVECTOR3 m_vOffsetPos = m_vPosition;
	
	m_transform = m_scale * m_rotation * m_translate;
	pSprite->SetTransform(&m_transform);
	switch(m_Direction){
		case NONE:
			pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vPosition, 0xFFFFFFFF); 
			break;
		case HORIZONTAL:
			m_vOffsetPos.x+= m_xOffset;
			pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vPosition, m_ModulateColor); 
			pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vOffsetPos, m_ModulateColor); 
			break;
		case VERTICAL:
			m_vOffsetPos.y+= m_yOffset;
			pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vPosition, 0xFFFFFFFF); 
			pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vOffsetPos, 0xFFFFFFFF); 
			break;
		case BOTH:
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vPosition, 0xFFFFFFFF); 
			//m_vOffsetPos = m_vPosition;
			//m_vOffsetPos.x+= m_xOffset;
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vOffsetPos, 0xFFFFFFFF); 
			//m_vOffsetPos = m_vPosition;
			//m_vOffsetPos.y+= m_yOffset;
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vOffsetPos, 0xFFFFFFFF); 
			//m_vOffsetPos.x+= m_xOffset;
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vOffsetPos, 0xFFFFFFFF); 
			break;
	}
	D3DXMatrixIdentity(&m_transform);
	pSprite->SetTransform(&m_transform);
}
void CBackground::Update(){
	// Sets the Scroll Direction for the correct
	// Drawing mechanism
	if(m_xVelocity == 0 && m_yVelocity == 0)
			m_Direction = NONE;
	if(m_xVelocity != 0 && m_yVelocity == 0)
			m_Direction = HORIZONTAL;
	if(m_xVelocity == 0 && m_yVelocity != 0)
			m_Direction = VERTICAL;
	if(m_xVelocity != 0 && m_yVelocity != 0)
			m_Direction = BOTH;
	// Updates Position
	m_vPosition.x += m_xVelocity;
	m_vPosition.y += m_yVelocity;
}
