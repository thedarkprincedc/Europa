#include "EuropaLogic.h"

BOOL BoundingBoxCollision(CEntity *objectA, CEntity *objectB){
	RECT rcA = objectA->GetSprite()->GetRect();
	RECT rcB = objectB->GetSprite()->GetRect();
	POINT p;
	p.x = (rcB.left);
	p.y = (rcB.top);
	return PtInRect(&rcA, p);
	//return FALSE;
}
BOOL EuropaLogic::CheckCollision(){
	// Check Collision
	for(int i = 0; i < m_vEntities.size(); i++){
		for(int j = 0; j < m_vEntities.size(); j++){
			//Skip Entity if the same
			if(m_vEntities[i] != m_vEntities[j]){
				if(BoundingBoxCollision(m_vEntities[i], m_vEntities[j]) == TRUE){
					CollisionHandler(m_vEntities[i], m_vEntities[j]);
					::OutputDebugString("Collision Happened\n");
				}
			}
		}
	}
	return TRUE;
}
EuropaLogic::EuropaLogic(void)	: m_PlayerScore(0) {	}
EuropaLogic::~EuropaLogic(void){	}
void EuropaLogic::Logic(){
	// Keep track of Points to make level changes
	// Change game state
	// Change spawn rate based on time / points
	// Tally Points 
	// Change Level 
}
void EuropaLogic::Frame(){
	CheckCollision();
}
ENTITYMAP EuropaLogic::GetEntityMap(){	
	return m_mEntityTypes;	
}
vector<CEntity*> *EuropaLogic::GetEntities(){	
	return &m_vEntities;	
}
int EuropaLogic::GetScore(){ 
	return m_PlayerScore;
}
EventDispacher *EuropaLogic::GetEventDispatcher(){
	return &pEvDispatch;
}
void EuropaLogic::DispatchEvents(IDirect3DDevice9 *pDevice){
	pEvDispatch.Dispatch(pDevice);
}