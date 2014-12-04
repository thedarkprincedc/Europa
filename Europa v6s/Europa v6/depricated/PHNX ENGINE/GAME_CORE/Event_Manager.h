#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_
#include "./Core_Global.h"
#include <fstream>
#include <sstream>
#include <string>
#include <hash_map>
#include <queue>
using namespace std;
using namespace stdext;
typedef enum { NOTPROCESSED, SUCESSFUL, UNSUCESSFUL } EVENTSTATUS;

class Event{	// Event Message
public:	
	Event(string pCommand, int nRefCount = 1);	 // Reference Countr
	~Event();
	string GetCommand();					 // GetCommand
	void SetCommand(string szCommand);		 // SetCommand
	void SetStatus(EVENTSTATUS eventStatus); // SetStatus
	EVENTSTATUS GetStatus();				 // GetStatus
protected:
	string		m_szCommand;		// Command
	int			m_nRefCounter;		// Counter 
	EVENTSTATUS m_EventStatus;		// Event Status
};
class EventListener{	// Event Lisener
public:	
	virtual BOOL ProcessEvent(IDirect3DDevice9 *pDevice, Event *pEvent) = 0;
};
#define MAKELOWERCASE(String) for(int i = 0; i < String.size(); i++) String.at(i) = tolower(String.at(i))
class EventDispacher{
public:
	void AddEventListener(string pCommandTag, EventListener *pEventListener){
		MAKELOWERCASE(pCommandTag);		
		char *pChar = NULL;
		if((pChar = strtok((char*)pCommandTag.c_str(), " ")) != NULL){ // Get CommandTag
			do{	// Insert commandTag, and Event Listener
				m_multiEvent.insert(make_pair(pChar, pEventListener)); 
			}while((pChar = strtok(NULL, " ")) != NULL);
		}
	}
	BOOL ProcessCommand(string szCommand){
		MAKELOWERCASE(szCommand);			
		multimap<string, EventListener*>::iterator iterMultiEvent;
		char token[32];
		sscanf((char*)szCommand.c_str(), "%s", &token);
		if((iterMultiEvent = m_multiEvent.find(token)) != m_multiEvent.end()){
			AddEvent(szCommand);
			return TRUE;
		}
		::OutputDebugString(L"Command Failed");
		return FALSE;
	}
	void AddEvent(string szCommandEvent){ // Add Events to EventList
		MAKELOWERCASE(szCommandEvent);	
		m_qEventList.push(new Event(szCommandEvent));
	}
	void Dispatch(IDirect3DDevice9 *pDevice){
		if(m_qEventList.size() > 0){
			char token[32];
			multimap<string, EventListener*>::iterator  iterMulti;
			while(m_qEventList.size()>0){
				sscanf((char*)m_qEventList.front()->GetCommand().c_str(), "%s", &token);
				if((iterMulti = m_multiEvent.find(token)) != m_multiEvent.end()){
					iterMulti->second->ProcessEvent(pDevice, m_qEventList.front());
					m_qEventList.pop();
				}
			}
		}
	}
protected:
	queue<Event*>					  m_qEventList;
	vector<string>					  m_vCommands;
	multimap<string, EventListener*>  m_multiEvent;
};
#endif