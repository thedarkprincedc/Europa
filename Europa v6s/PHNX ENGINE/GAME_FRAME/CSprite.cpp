#include "./Global_Framework.h"
CSprite::CSprite(IDirect3DDevice9 *pDevice, char *pImgFilename, 
				 D3DCOLOR KeyColor, float x, float y)
	:	CPanelEx(pDevice, pImgFilename, KeyColor, x, y){	}
CSprite::CSprite(IDirect3DDevice9 *pDevice, 
				 IDirect3DTexture9 *pTexture, float x, float y)
	:	CPanelEx(pDevice, pTexture){	}
CSprite::~CSprite(){	}
HRESULT CSprite::Load(char *pImgFilename, D3DCOLOR color){	return S_OK;  }
HRESULT CSprite::Load(IDirect3DTexture9 *pTexture){	return S_OK; }
void CSprite::Render(IDirect3DDevice9 *pDevice){  CPanelEx::Render(pDevice);  }
RECT CSprite::GetRect(){
	/*RECT rc = { CPanelEx::getX(), 
				CPanelEx::getY(), 
				CPanelEx::getX() + CPanelEx::getWidth(),
				CPanelEx::getY() + CPanelEx::getHeight()};
	*/
	RECT rc = { getX(), getY(), getX() + getWidth(), getY() + getHeight() };
	return rc;
}
/*
RITTI CSprite::getRITTI(){ return RITTI::SPRITE; }
RECT CSprite::getRect(){
	RECT rc;
	ZeroMemory(&rc, sizeof(RECT));
	if(m_pTexture != NULL){
		D3DSURFACE_DESC desc;				// Create Surface Description
		m_pTexture->GetLevelDesc(0, &desc);	// Get Surface Description
		m_TextureWidth = desc.Width;		// Get Width
		m_TextureHeight = desc.Height;		// Get Height
	}
	rc.top    = m_vTranslation.y; 
	rc.left	  = m_vTranslation.x;
	rc.right  = m_vTranslation.x + m_TextureWidth;
	rc.bottom = m_vTranslation.y + m_TextureHeight;
	return rc;
}
CSprite::CSprite(IDirect3DDevice9 *pDevice,	ID3DXSprite *pSprite, 
				 char *pImgFilename, D3DCOLOR color)
	:	m_pSprite(pSprite), m_pDevice(pDevice){
	ZeroMemory(&m_SourceRect, sizeof(RECT));
	Load(pImgFilename, color);
}
CSprite::CSprite(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,
		IDirect3DTexture9 *pTexture, D3DCOLOR color)
	:	m_pSprite(pSprite), m_pDevice(pDevice) {
	ZeroMemory(&m_SourceRect, sizeof(RECT));
	Init();
	m_pTexture = pTexture;
	if(m_pTexture != NULL){
		D3DSURFACE_DESC desc;				// Create Surface Description
		m_pTexture->GetLevelDesc(0, &desc);	// Get Surface Description
		m_TextureWidth = desc.Width;		// Get Width
		m_TextureHeight = desc.Height;		// Get Height
	}
}
CSprite::~CSprite(){

}
void CSprite::Init(){
	m_TextureWidth		=	0;
	m_TextureHeight		=	0;
	m_fRotation			=	0.f;
	m_vRotationPoint	=	D3DXVECTOR2(0,0);
	m_vScale			=	D3DXVECTOR2(1.0,1.0);
	m_vTranslation		=	D3DXVECTOR2(0,0);
	m_ColorKey			=	D3DCOLOR_XRGB(0,0,0);
	m_ModulateColor		=	D3DCOLOR_XRGB(255,255,255);
	ZeroMemory(&m_SourcePosition, sizeof(D3DXVECTOR3));
}
HRESULT CSprite::Load(char *pImgFilename, D3DCOLOR color){
	Init();
	m_pTexture = CSpriteManager::get()->getSprite(pImgFilename, m_pDevice, color);
	if(m_pTexture == NULL)
		return E_FAIL;
	if(m_pTexture != NULL){
		D3DSURFACE_DESC desc;				// Create Surface Description
		m_pTexture->GetLevelDesc(0, &desc);	// Get Surface Description
		m_TextureWidth = desc.Width;		// Get Width
		m_TextureHeight = desc.Height;		// Get Height
	}
	return S_OK;
}
HRESULT CSprite::Load(IDirect3DTexture9 *pTexture){
	m_pTexture = pTexture;				// Set Texture
	D3DSURFACE_DESC desc;				// Create Surface Description
	m_pTexture->GetLevelDesc(0, &desc);	// Get Surface Description
	m_TextureWidth = desc.Width;		// Get Width
	m_TextureHeight = desc.Height;		// Get Height
	return S_OK;
}
void CSprite::Render(){
	D3DXVECTOR3 m_vRotationPointNew(m_vRotationPoint.x,			// Set New Rotation Point
							  m_vRotationPoint.y, 0.0f);	
	D3DXMATRIX mat, matScale, matRotaZ, matTrans, OriginalMat;	// Create transformation
																// matricies
	D3DXMatrixIdentity(&mat);									// Get Identity Matrix #1
	D3DXMatrixIdentity(&OriginalMat);							// Get Identity Matrix #2
	D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, 1.0f);	// Set Scaling Matrix
	D3DXMatrixRotationZ(&matRotaZ, m_fRotation);				// Set Z Rotation
	//*********Set Translation Matrix************
	D3DXMatrixTranslation(&matTrans, m_vTranslation.x, m_vTranslation.y, 0.0f);
	//****		Multiply Matices		****
	D3DXMatrixMultiply(&mat, &mat, &matRotaZ);	// Rotation	
	D3DXMatrixMultiply(&mat, &mat, &matTrans);	// Translation
	D3DXMatrixMultiply(&mat, &mat, &matScale);	// Scale
	m_pSprite->SetTransform(&mat);				// Set Transformation to(Matrix)
	if((m_SourceRect.left == 0) && (m_SourceRect.top == 0) &&
	   (m_SourceRect.right == 0) && (m_SourceRect.bottom == 0))
		m_pSprite->Draw(m_pTexture, NULL, &m_vRotationPointNew, NULL, m_ModulateColor); 
	else
		m_pSprite->Draw(m_pTexture, &m_SourceRect, &m_vRotationPointNew, 
						&m_SourcePosition, m_ModulateColor);
	m_pSprite->SetTransform(&OriginalMat);		// Set Transformation Matrix to Identity
}
void CSprite::SetTranslation(D3DXVECTOR2 translation){
	m_vTranslation = translation;	
}
void CSprite::SetTranslation(float x, float y){
	m_vTranslation.x = x; 
	m_vTranslation.y = y;
}
void CSprite::GetTranslation(D3DXVECTOR2 *translation){
	*translation = m_vTranslation;	
}
void CSprite::SetRotationByDegree(float rotationDegree){
	SetRotation(D3DXToDegree(rotationDegree));		
}
void CSprite::GetRotationByDegree(float *rotationDegree){
	*rotationDegree = D3DXToDegree(m_fRotation); 
}
void CSprite::SetRotation(float rotation){	 m_fRotation = rotation;	}
void CSprite::GetRotation(float *rotation){	*rotation = m_fRotation;	}
void CSprite::SetRotationPoint(D3DXVECTOR2 rotationPoint){
	m_vRotationPoint = rotationPoint;	
}
void CSprite::GetRotationPoint(D3DXVECTOR2 *rotationPoint){
	*rotationPoint = m_vRotationPoint;	
}
void CSprite::SetScale(D3DXVECTOR2 scale){	m_vScale = scale;	 }
void CSprite::GetScale(D3DXVECTOR2 *scale){	*scale = m_vScale;	 }
void CSprite::SetColorKey(D3DCOLOR color){	m_ColorKey = color;	 }
void CSprite::GetColorKey(D3DCOLOR *color){	*color = m_ColorKey; }
void CSprite::SetModulateColor(D3DCOLOR color){	 m_ModulateColor = color;  }
void CSprite::GetModulateColor(D3DCOLOR *color){ *color = m_ModulateColor; }
float CSprite::getWidth(){	return m_TextureWidth;	}
float CSprite::getHeight(){	return m_TextureHeight;	}
float CSprite::getX(){	return m_vTranslation.x;	}
float CSprite::getY(){	return m_vTranslation.y;	}
//***************************************************
*/
CAnimTimer::CAnimTimer(long i): m_TimeInterval(i), m_lastTime(0){}
void CAnimTimer::SetTime(long i){ m_TimeInterval = i; }
BOOL CAnimTimer::Run(){
	if(GetTickCount() - m_lastTime < m_TimeInterval)
		return FALSE;
	m_lastTime = GetTickCount();
	return TRUE;
}
/*
//***************************************************
CAnimSprite::CAnimSprite(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
						 char *pImgfilename, D3DCOLOR color)
	:	CSprite(pDevice, pSprite, pImgfilename, color), 
	m_OffsetX(0), m_OffsetY(0), nextframe(0){
}
void CAnimSprite::SetActiveSprite(int frameNum){
	int SpritesPerRow = getWidth() / m_nFrameWidth;
	if(frameNum > m_nNumOfFrames) frameNum = 0;
	div_t Result;
	Result = div(frameNum, SpritesPerRow);
	m_OffsetX = Result.rem*m_nFrameWidth;
	m_OffsetY = Result.quot*m_nFrameHeight;
	//m_OffsetX = (frameNum%SpritesPerRow)*m_nFrameWidth;
	//m_OffsetY = (frameNum/SpritesPerRow)*m_nFrameHeight;
}
void CAnimSprite::SetupAnimation(int numFrames, int frameWidth, int frameHeight, 
		BOOL playOnce, long timeIntervalMillisec, ANIMDIRECTION directionAnim){
	m_nNumOfFrames = numFrames-1;
	m_nFrameWidth = frameWidth;
	m_nFrameHeight = frameHeight;
	m_bPlayOnce = playOnce;
	m_Timer = new CAnimTimer(timeIntervalMillisec);
	m_AnimationDirection = directionAnim;
	m_nCurrFrame = (directionAnim == REVERSE) ? numFrames : 0;
	m_SourceRect.left = m_SourceRect.top = 0;
	m_SourceRect.right = frameWidth;
	m_SourceRect.bottom = m_nFrameHeight;
	//CSprite::m_TextureHeight = frameHeight;
	//CSprite::m_TextureWidth = frameWidth;
}
void CAnimSprite::SetOffsets(){
	m_SourceRect.left = m_OffsetX;
	m_SourceRect.top = m_OffsetY;
	m_SourceRect.right = (m_nFrameWidth + m_OffsetX);
	m_SourceRect.bottom = (m_nFrameHeight + m_OffsetY);
}
void CAnimSprite::Forward(){
	if(!m_Timer->Run())
		return;
	SetOffsets();
	SetActiveSprite(m_nCurrFrame);
	m_nCurrFrame = (m_nCurrFrame < m_nNumOfFrames) ? (m_nCurrFrame+=1) : 0;
}
void CAnimSprite::Reverse(){}
void CAnimSprite::Oscillate(){
	if(!m_Timer->Run())
		return;
	SetOffsets();
	SetActiveSprite(m_nCurrFrame);
	if(m_nCurrFrame == 0)	nextframe = +1;
	if(m_nCurrFrame == m_nNumOfFrames)	nextframe = -1;
	m_nCurrFrame += nextframe;
}
void CAnimSprite::Render(){
	switch(m_AnimationDirection){
		case FORWARD:	Forward();	 break;
		case REVERSE:	Reverse();	 break;
		case OSCILLATE:	Oscillate(); break;
	}
	CSprite::Render();
}
*/