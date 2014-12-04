#include "Core_Global.h"
Event::Event(string pCommand, int nRefCount)
	:	m_szCommand(pCommand), 
	m_nRefCounter(nRefCount), 
	m_EventStatus(NOTPROCESSED){ }
Event::~Event(){	}
string Event::GetCommand(){ return m_szCommand; }
void Event::SetCommand(string szCommand){ m_szCommand = szCommand; }
void Event::SetStatus(EVENTSTATUS eventStatus){ m_EventStatus = eventStatus; }
EVENTSTATUS Event::GetStatus(){	return m_EventStatus; }
// Event Dispatcher
