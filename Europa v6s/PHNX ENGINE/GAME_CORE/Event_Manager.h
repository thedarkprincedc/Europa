#ifndef EVENTMANAGER_H_
#define EVENTMANAGER_H_
#include "./Core_Global.h"
#include <fstream>
#include <sstream>
#include <string>
#include <hash_map>
using namespace std;
using namespace stdext;

typedef enum {NOTPROCESSED, SUCESSFUL, UNSUCESSFUL} EVENTSTATUS;
class Event{	// Event Message
public:	
	Event(string pEventType, string pCommand);		
	~Event();
	void SetEvent(string pEventType, string pCommand);
	string GetEventType();
	string GetCommand();
	void SetStatus(EVENTSTATUS eventStatus);
	EVENTSTATUS GetStatus();
protected:
	string		m_szEventType; 
	string		m_szCommand;
	EVENTSTATUS m_EventStatus;
};

class EventListener{	// Event Lisener
public:	
	virtual BOOL ProcessEvent(IDirect3DDevice9 *pDevice, Event *pEvent) = 0;
};

class EventDispacher{
public:
	void SendEvent(char *pEventType, char *pCommand);
	BOOL ProcessCommand(char *pCommand); // Processes a command into a message
	void AddEventListener(char *pEventType, EventListener *pEventListener);
	void AddNewCommand(string Command, string CommandType);
	void Dispatch(IDirect3DDevice9 *pDevice);
protected:
	map<string, string>					 m_CommandList;	// Command List
	vector<Event*>						 m_pEvent;	// Vector of Events
	map<string, vector<EventListener*> > m_mapEventNew; // EventListeners
};
#endif