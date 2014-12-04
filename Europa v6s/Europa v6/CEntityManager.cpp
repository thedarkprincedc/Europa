#include "./CEntityManager.h"
CEntityManager::CEntityManager(IDirect3DDevice9 *pDevice, 
	vector<CEntity*> *pEntity)	:	m_pEntity(pEntity){	}
void CEntityManager::Render(IDirect3DDevice9 *pDevice){
	for(vector<CEntity*>::iterator it = m_pEntity->begin();
		it != m_pEntity->end(); it++){
		(*it)->Render(pDevice);
	}
	return;
}

BOOL CEntityManager::ProcessEvent(IDirect3DDevice9 *pDevice, Event *pEvent){
	string s;
	sscanf(pEvent->GetCommand().c_str(), "%s", s.c_str());
	static DWORD transColor = NULL;
	MATCH(s.c_str(), "SetTransColor"){
		int r = 0 , g = 0, b = 0;
		sscanf(pEvent->GetCommand().c_str(), "%s %d %d %d", s.c_str(), &r, &g, &b);
		transColor = D3DCOLOR_XRGB( r, g, b);
		return TRUE;		
	}
	MATCH(s.c_str(), "CreateEntity"){
		char filepath[64], aitype[32];
		ZeroMemory(&filepath, sizeof(filepath));
		int x = 0, y = 0;
		sscanf(pEvent->GetCommand().c_str(), "%s %s %s %d %d", s.c_str(), filepath, 
			aitype, &x, &y);
		CBaseBehavior *pBaseBehavior = NULL;
		MATCH(aitype, "HUMANAI") pBaseBehavior = new CHumanAi();
		MATCH(aitype, "BULLETAI") pBaseBehavior = new CBullet();
		m_pEntity->push_back(new CEntity(pDevice, filepath, pBaseBehavior, 
			x, y, transColor));
		return TRUE;		
	}
	MATCH(s.c_str(), "DeleteAllEnt"){
		m_pEntity->clear();
		return TRUE;
	}
	return FALSE;
}
//*****************************************************
//*****************************************************
void CBaseBehavior::Init(CEntity *pEntity){	m_pEntity = pEntity; }
void CHumanAi::Process(){
	if(GetAsyncKeyState(VK_LEFT))
		this->m_pEntity->GetSprite()->TranslateRel(-1,0);
	if(GetAsyncKeyState(VK_RIGHT))
		this->m_pEntity->GetSprite()->TranslateRel(1,0);
	if(GetAsyncKeyState(VK_UP))
		this->m_pEntity->GetSprite()->TranslateRel(0,1);
	if(GetAsyncKeyState(VK_DOWN))
		this->m_pEntity->GetSprite()->TranslateRel(0,-1);
	if(GetAsyncKeyState(VK_SPACE)){
		switch(m_pEntity->GetIsAlive()){
			case TRUE:
				this->m_pEntity->SetIsAlive(FALSE);
				break;
			case FALSE:
				this->m_pEntity->SetIsAlive();
				break;
		}
	}	
}
void CBullet::Process(){
	m_pEntity->GetSprite()->TranslateRel(1,0);
}
CEntity::CEntity(IDirect3DDevice9 *pDevice, char *pTextureFilename, 
				 CBaseBehavior *pBehavior, float x, float y,
				 D3DCOLOR KeyColor, BOOL bAlive)
	:	m_pSprite(pDevice, pTextureFilename, KeyColor, x, y),
		m_pBehavior(pBehavior){
	m_strRITTI = NULL;
	if(pBehavior != NULL)
		m_pBehavior->Init(this);
}
void CEntity::SetRitti(char *pString){
	m_strRITTI = pString;	
}
char *CEntity::GetRitti(){
	return m_strRITTI;	
}
CSprite *CEntity::GetSprite(){	
	return &m_pSprite;	
}
void CEntity::SetIsAlive(BOOL bisAlive){	
	m_bIsAlive = bisAlive; 
}
BOOL CEntity::GetIsAlive(){	
	return m_bIsAlive;	
}
void CEntity::Render(IDirect3DDevice9 *pDevice){
	if(m_bIsAlive){
		m_pSprite.Render(pDevice);
		if(m_pBehavior != NULL)
			m_pBehavior->Process();
	}
}