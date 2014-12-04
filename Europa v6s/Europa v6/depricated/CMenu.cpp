#include "CMenu.h"
CMenu::CMenu(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, CStateManager *pStateManager)
	:   m_pDevice(pDevice), m_pSprite(pSprite), m_pStateManager(pStateManager), 
	    m_MenuXPos(0), m_MenuYPos(0), m_MouseXPos(0), m_MouseYPos(0){}
CMenu::~CMenu(){ delete m_pStateManager;}
CObject *CMenu::CheckSelection(int x, int y){
	POINT pt = {x,y};
	RECT rc;
	OBJECTMAP::iterator iterObject;
	for(iterObject = m_ObjectMap.begin(); 
		iterObject != m_ObjectMap.end(); ++iterObject){
		rc = (*iterObject).first->getRect();
		if(PtInRect(&(*iterObject).first->getRect(), pt) == TRUE)
			return (*iterObject).first;
	}
	return NULL;
}
void CMenu::AddMenuItem(char *ItemName, GAMESTATE Function, float x, float y){
	// If item name is not found in the map then
	if(m_IndexMap.find(ItemName) == m_IndexMap.end()){
		CFont *pfont = new CFont(m_pDevice, m_pSprite);
		pfont->SetTranslation(D3DXVECTOR2(x,y));	
		pfont->SetText(ItemName);		
		m_IndexMap[ItemName] = pfont;
		m_ObjectMap[pfont] = Function;
		AlignMenu();								
		pfont = 0;								
	}
}
void CMenu::AddIconItem(char *IconName, GAMESTATE Function, float x, float y){ 
	IDirect3DTexture9 *pTexture = NULL;
	if((pTexture = CSpriteManager::get()->getSprite(IconName, m_pDevice, NULL)) != NULL){
		m_ObjectMap[new CSprite(m_pDevice, m_pSprite, pTexture)] = Function;
	}
}
void CMenu::AddAnimatedIcon(char *IconName, GAMESTATE Function, 
		int numFrames, int frameWidth, int frameHeight, BOOL playOnce, 
		long timeIntervalMillisec, CAnimSprite::ANIMDIRECTION directionAnim, 
		float x, float y){
	IDirect3DTexture9 *pTexture = NULL;
	if((pTexture = CSpriteManager::get()->getSprite(IconName, m_pDevice, NULL)) != NULL){
		CAnimSprite *pAnimSprite = new CAnimSprite(m_pDevice, m_pSprite, IconName);
		pAnimSprite->SetupAnimation(numFrames, frameWidth, frameHeight, playOnce, 
									timeIntervalMillisec, directionAnim);
		m_ObjectMap[pAnimSprite] = Function;
	}
}
void CMenu::Frame(){
	D3DXMATRIX OriginalMat;
	D3DXMatrixIdentity(&OriginalMat);			// Get Identity Matrix #2
	POINT pt = {m_MouseXPos, m_MouseYPos};
	OBJECTMAP::iterator objMapIter;
	for(objMapIter = m_ObjectMap.begin(); objMapIter != m_ObjectMap.end(); ++objMapIter){
		if(PtInRect(&(*objMapIter).first->getRect(), pt) == TRUE)
			OnMouseOver((*objMapIter).first);
		else
			OnMouseOut((*objMapIter).first);
		(*objMapIter).first->Render();
	}
	m_pSprite->SetTransform(&OriginalMat);		// Set Transformation Matrix to Identity
}
void CMenu::SetTextSize(float TextSize){
	OBJECTMAP::iterator iter = m_ObjectMap.begin();
	for(iter = m_ObjectMap.begin(); iter != m_ObjectMap.end(); iter++){
		if((*iter).first->getRITTI() == FONT){
			CFont *pfont = (CFont*)(*iter).first;
			pfont->SetSize(TextSize);
		}
	}
}
void CMenu::AlignMenu(){
	float x = m_MenuXPos, y = m_MenuYPos;
	float offy = static_cast<float>(this->m_ObjectMap.begin()->first->getRect().bottom);
	OBJECTMAP::iterator m_ObjectIter;
	for(m_ObjectIter = m_ObjectMap.begin(); 
		m_ObjectIter != m_ObjectMap.end();
		++m_ObjectIter){
		if((*m_ObjectIter).first->getRITTI() == FONT)
			(*m_ObjectIter).first->SetTranslation(x, y);
		y += offy;
	}
}
void CMenu::SetMenuPosition(float x, float y){
	m_MenuXPos = x;	
	m_MenuYPos = y;
	AlignMenu();
}
void CMenu::TrackMouse(UINT uMsg, WPARAM wParam, LPARAM lParam){
	CObject *pObject;
	switch(uMsg){
		case WM_MOUSEMOVE:
			m_MouseXPos = LOWORD(lParam);
			m_MouseYPos = HIWORD(lParam);
		break;
		case WM_LBUTTONDOWN:	
			if((pObject = CheckSelection(LOWORD(lParam), HIWORD(lParam))) != NULL)
				OnMouseLeftClick(pObject);
		break;
	}
}
void CMenu::OnMouseOver(CObject *pFont){}
void CMenu::OnMouseOut(CObject *pFont){}
void CMenu::OnMouseLeftClick(CObject *pObject){
	OBJECTMAP::iterator iterObject;
	if(m_pStateManager != NULL){
		if((iterObject = m_ObjectMap.find(pObject)) != m_ObjectMap.end()){
			m_pStateManager->Push((iterObject)->second, NULL);
		}
	}
}
void CMenu::OnMouseRightClick(){}
void CMenu::deleteText(const char *pText){
	TEXTINDEXMAP::iterator textidx;
	if((textidx = m_IndexMap.find(pText)) !=m_IndexMap.end()){
		m_ObjectMap.erase(m_ObjectMap.find(textidx->second));
	}
}

//************************************************************

CMenuEx::CMenuEx(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
	    CStateManager *pStateManager)
	:  CMenu(pDevice, pSprite, pStateManager),
	OverAlpha(0){}
void CMenuEx::OnMouseOver(CObject *pObject){
	CFont *pfont = NULL;
	CAnimSprite *pAnimSprite = NULL;
	switch(pObject->getRITTI()){
		case FONT:
			pfont = (CFont*)pObject;
			pfont->SetColor(D3DCOLOR_ARGB(OverAlpha+=16,255,255,255));
		return;
		case SPRITE:
			//CSprite *pSprite = (CSprite*)pObject;
			pAnimSprite = (CAnimSprite*)pObject;
			pAnimSprite->Forward();
		return;
		//case ANIMATED:
		//return;
	}
}
void CMenuEx::OnMouseOut(CObject *pObject){
	CFont *pfont = NULL;
	CAnimSprite *pAnimSprite = NULL;
	switch(pObject->getRITTI()){
		case FONT:
		pfont = (CFont*)pObject;
		pfont->SetColor(D3DCOLOR_ARGB(255,255,255,255));	
		return;
	}
}