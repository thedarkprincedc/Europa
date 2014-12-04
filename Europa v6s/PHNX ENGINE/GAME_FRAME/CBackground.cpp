#include "./CBackground.h"
/*
CBackground::CBackground(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
						 char *pImgFilename, D3DCOLOR color)
		:	CSprite(pDevice, pImgFilename, color){
	m_Direction = NONE;
	m_vPosition = D3DXVECTOR3(0,0,0);
	m_xOffset = m_yOffset = 0;
	m_xVelocity = 0;
	m_yVelocity = 0;
	m_rcRect.left = m_rcRect.top = 0;
	m_rcRect.right = static_cast<long>(getWidth());
	m_rcRect.bottom = static_cast<long>(getHeight());
}
CBackground::~CBackground(){}
HRESULT CBackground::Load(char *pImgFilename, D3DCOLOR color){
	return CSpriteEx::Load(pImgFilename, color);
}
void CBackground::setDirectionAbs(int x, int y){
	m_xVelocity = x;
	m_yVelocity = y;
}
void CBackground::Render(){
	m_xOffset = (m_vPosition.x < 0) ? getWidth() : -getWidth();
	m_vPosition.x =(m_vPosition.x < -getWidth() || m_vPosition.x > getWidth()) ? 0 : m_vPosition.x;
	m_yOffset=(m_vPosition.y<0)?getHeight():-getHeight();
	m_vPosition.y =(m_vPosition.y < -getHeight()||m_vPosition.y > getHeight()) ? 0 : m_vPosition.y;	
	D3DXVECTOR3 m_vOffsetPos = m_vPosition;
	switch(m_Direction){
		case NONE:
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vPosition, 0xFFFFFFFF); 
			break;
		case HORIZONTAL:
			m_vOffsetPos.x+= m_xOffset;
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vPosition, 0xFFFFFFFF); 
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vOffsetPos, 0xFFFFFFFF); 
			break;
		case VERTICAL:
			m_vOffsetPos.y+= m_yOffset;
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vPosition, 0xFFFFFFFF); 
			//m_pSprite->Draw(m_pTexture, &m_rcRect, 0, &m_vOffsetPos, 0xFFFFFFFF); 
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
}
void CBackground::Frame(){
	Update();
	Render();
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
*/

/*************************************
CBackground Parseable
**************************************/
/*
CBackgroundParseable::CBackgroundParseable(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,	
			  char *pImgFilename, D3DCOLOR color)
	: CBackground(pDevice, pSprite, pImgFilename, color){
	CCommandParser::get()->addClass(this);
}
CBackgroundParseable::~CBackgroundParseable(){}
BOOL CBackgroundParseable::ParserCallback(char *pString, char *pOutput){
	BOOL bSuccess = FALSE;
	char *text = strtok(pString, " =");
	while(text != NULL){
		MATCH(text, "LoadBack"){
			text = strtok(NULL, " =;");
			if(!FAILED(this->Load(text, NULL))){
				strcat(pOutput, "Success");
				bSuccess = TRUE;
			}
		}
		text = strtok(NULL," =\n");
	}
	return bSuccess;
}
*/