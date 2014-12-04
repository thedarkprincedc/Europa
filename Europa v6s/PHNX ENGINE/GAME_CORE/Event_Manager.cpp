#include "Core_Global.h"
Event::Event(string pEventType, string pCommand){
	SetEvent(pEventType, pCommand);
	m_EventStatus = NOTPROCESSED;
}
Event::~Event(){	}
void Event::SetEvent(string pEventType, string pCommand){
	m_szEventType = pEventType;
	m_szCommand = pCommand;							
}
string Event::GetEventType(){ return m_szEventType;	}
string Event::GetCommand(){	  return m_szCommand;	}
void Event::SetStatus(EVENTSTATUS eventStatus){
	m_EventStatus = eventStatus;					
}
EVENTSTATUS Event::GetStatus(){	return m_EventStatus; }
void EventDispacher::SendEvent(char *pEventType, char *pCommand){
	m_pEvent.push_back(new Event(pEventType, pCommand));
}
BOOL EventDispacher::ProcessCommand(char *pCommand){
	string k = pCommand;
	map<string, string>::iterator itMap;
	char *p = NULL;
	if((p = strtok(pCommand, " ")) != NULL){
		if((itMap = m_CommandList.find(p)) != m_CommandList.end()){
			strcpy(pCommand, (char*)k.c_str());
			SendEvent((char*)itMap->second.c_str(), (char*)k.c_str());
			return TRUE;
		}
		else {
			::OutputDebugString("Unknown Command\n");
			return FALSE;
		}
	}
}
void EventDispacher::AddEventListener(char *pEventType, EventListener *pEventListener){
	m_mapEventNew[pEventType].push_back(pEventListener);
}
void EventDispacher::AddNewCommand(string Command, string CommandType){
	m_CommandList[Command] = CommandType;
}
void EventDispacher::Dispatch(IDirect3DDevice9 *pDevice){
	if(m_pEvent.size() > 0){ // if events exists process it
		map<string, vector<EventListener*> >::iterator iterMap;
		BOOL bSuccess = FALSE;
		for(int i = 0; i < m_pEvent.size(); i++){ 
			if((iterMap = m_mapEventNew.find(m_pEvent[i]->GetEventType())) != m_mapEventNew.end()){
				for(int k = 0; k < iterMap->second.size(); k++){
					if((bSuccess = (*iterMap).second[k]->ProcessEvent(pDevice, m_pEvent[i])) == TRUE){
						m_pEvent.erase((m_pEvent.begin()+i));
					}
				}
			}
		}
	}
}