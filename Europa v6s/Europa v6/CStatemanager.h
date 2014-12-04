#pragma once
//**************************************************
enum Purpose{ NOPURPOSE = 0, INITPURPOSE, SHUTDOWNPURPOSE, FRAMEPURPOSE };
/************************************
	CStateManager
************************************/
class CStateManager{
	typedef struct sState{
		void (*Function)(void *Ptr, long Purpose);
		sState *Next; 
		sState(){ Function = NULL; Next = NULL; }
		~sState(){ delete Next;	}
	}sState;
protected:
	sState *m_StateParent;
public:
	CStateManager();
	~CStateManager();
	void Push(void (*Function)(void *Ptr, long Purpose), void *DataPtr = NULL);
	void PushTransition(void (*Function)(void *Ptr, long Purpose), int nRunningTime, void *DataPtr = NULL);
	BOOL Pop(void *DataPtr = NULL);
	void PopAll(void *DataPtr = NULL);
	BOOL Process(void *DataPtr = NULL);
};
CStateManager::CStateManager() : m_StateParent(NULL){}
CStateManager::~CStateManager(){ PopAll(); }
void CStateManager::Push(void (*Function)(void *Ptr, long Purpose), void *DataPtr){
	sState *StatePtr;
	if(Function != NULL){
		StatePtr = new sState();
		StatePtr->Function = Function;
		StatePtr->Next = m_StateParent;
		m_StateParent = StatePtr;
		StatePtr->Function(DataPtr, INITPURPOSE); // Run Init Purpose
	}
}
BOOL CStateManager::Pop(void *DataPtr){
	sState *StatePtr;
	if((StatePtr = m_StateParent) != NULL){
		m_StateParent->Function(DataPtr, SHUTDOWNPURPOSE);
		m_StateParent = StatePtr->Next;
		StatePtr->Next = NULL;
		delete StatePtr;
	}
	if(m_StateParent == NULL)	return FALSE;
	return TRUE;
}

void CStateManager::PopAll(void *DataPtr){	while(Pop(DataPtr) == TRUE); }
BOOL CStateManager::Process(void *DataPtr){
	if(m_StateParent == NULL)
		return FALSE;
	m_StateParent->Function(DataPtr, FRAMEPURPOSE);
	return TRUE;
}