#include "LevelTransition.h"
/*
CLevelTransition::CLevelTransition(void){	m_bShowTrans = TRUE;	}
CLevelTransition::~CLevelTransition(void){							}
void CLevelTransition::LoadTransition(IDirect3DDevice9 *pDevice, 
									  char *pszBackground, long millisec){
	m_pBackground = new CBackground(pDevice, pszBackground);
	m_nMilliSec = millisec;
	m_bShowTrans = TRUE;
	m_nLastTime = 0;
	gAnimTimer = new CObjectTimer(800);
}
void CLevelTransition::Show(BOOL bShow){ m_bShowTrans = bShow; }
HRESULT CLevelTransition::Render(IDirect3DDevice9 *pDevice){
	if(m_bShowTrans){
		if(gAnimTimer->Run()){
			Show(FALSE);
		}
		m_pBackground->Frame(pDevice);
	}
	return S_OK;
}
*/