#ifndef _CENTITYMANAGER_H_
#define _CENTITYMANAGER_H_
#include "../PHNX ENGINE/phoenixengine.h"
#define PLAYERENTITY "Player Entity"
#define ENEMYENTITY  "Enemy Entity"
#include <sstream>
using namespace std;
class CEntity;
class CBaseBehavior;
//typedef map<DWORD, CEntity*> MAPOFENTITIY;

class CEntityManager : public EventListener{
public:
	CEntityManager(IDirect3DDevice9 *pDevice, vector<CEntity*> *pEntity);
	BOOL ProcessEvent(IDirect3DDevice9 *pDevice, Event *pEvent);
	void Render(IDirect3DDevice9 *pDevice);
	int GetNumOfEntities(){	return m_pEntity->size();	}
protected:
	//int					 m_nOfEntities;
	//map<DWORD, CEntity*> m_EntitiesMap;
	vector<CEntity*>	 *m_pEntity;
};	

class CBaseBehavior{
protected:
	CEntity *m_pEntity; // Store an Entity to modify
	CEntity *m_pTarget;	// Store a Target Entity
public:
	void Init(CEntity *pEntity);
	virtual void Process() = 0;
};
class CHumanAi : public CBaseBehavior{	
	public:	void Process();	
};
class CBullet : public CBaseBehavior{	
	public:	void Process();
};

class CEntity{	// Base Entity Class to derive from
protected:
	CBaseBehavior	*m_pBehavior;
	CSprite			 m_pSprite;
	BOOL			 m_bIsAlive;
	char		    *m_strRITTI;
	int				 m_nEntityID;
public:
	CEntity(IDirect3DDevice9 *pDevice, char *pTextureFilename, 
		CBaseBehavior *pBehavior, float x = 0, float y = 0, 
		D3DCOLOR KeyColor = NULL, BOOL bAlive = TRUE);
	void Render(IDirect3DDevice9 *pDevice);
	CSprite *GetSprite();
	CBaseBehavior *GetBehavior();
	void SetRitti(char *pString);
	char *GetRitti();
	void SetIsAlive(BOOL bisAlive = TRUE);
	BOOL GetIsAlive();
};
#endif