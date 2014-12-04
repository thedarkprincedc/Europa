#pragma once
#include <windows.h>
#include "../PHNX ENGINE/phoenixengine.h"
#include "./CEntityManager.h"

//void PixelPerfectCollision();
typedef vector<CEntity*> ENTITYVECTOR;
typedef map<string, ENTITYVECTOR > ENTITYMAP;
class Object;
class ObjectAllocate{
public:
	ObjectAllocate();
	~ObjectAllocate();
	vector<int> *GetListOfID();					// Get ID List
	vector<Object*>	*GetObjectVector();			// Get Object Vector
	map<int, vector<Object*> > *GetObjectMap(); // Get a pointer to object map
	void AddObject(int ID, Object *pObject);
protected:
//void CreateNewObject(int ID, Object *pObject);
	// Object Map = This map is actually a tree
	// The index [0] in each vector is a parent object
	// [++x] are subobjects like bullets
	map<int, map<int, Object*> > m_mapObject;
};
class EuropaLogic{	// This is where the game play out its RULES
public:
	EuropaLogic(void);
	~EuropaLogic(void);
	void Logic();
	void Frame();
	vector<CEntity*> *GetEntities();
	ENTITYMAP GetEntityMap();
	int GetScore();
	EventDispacher *GetEventDispatcher();
	void DispatchEvents(IDirect3DDevice9 *pDevice);
	BOOL CheckCollision();
	void CollisionHandler(CEntity *pEntityA, CEntity *pEntityB){

	}
protected:
	vector<string>		m_vLevelText;
	vector<CEntity*>	m_vEntities;
	map<string, vector<CEntity*> > m_mEntities;		// n# Entities and Type
	map<string, vector<CEntity*> > m_mEntityTypes;	// Entity Types
	// Event 
	EventDispacher		pEvDispatch;
	// Player Specific Option
	int					m_PlayerScore;
};