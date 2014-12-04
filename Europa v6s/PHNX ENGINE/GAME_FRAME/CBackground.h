#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
#include "./Global_Framework.h"

class CBackground : public EventListener{
	typedef enum { NONE = 0, HORIZONTAL = 1, VERTICAL = 2, BOTH = 3} MOVEMENT;
public:
	CBackground(IDirect3DDevice9 *pDevice, char *pImgFilename,
		float xVelocity = 0, float yVelocity = 0)
		:	m_pPanel(pDevice, pImgFilename), m_Direction(NONE){
			SetDirectionVelocity(xVelocity, yVelocity);
			m_xOffset   = m_yOffset = 0;
			m_xPosition = m_yPosition = 0;
	}
	~CBackground(){}
	void Load(IDirect3DDevice9 *pDevice, char *pImgFilename,
		float xVelocity = 0, float yVelocity = 0){
		m_pPanel.SetTexture(pDevice, pImgFilename);
	}
	void SetDirectionVelocity(float xVelocity = 0, float yVelocity = 0){
		m_xVelocity = xVelocity;
		m_yVelocity = yVelocity;
	}
	BOOL ProcessEvent(IDirect3DDevice9 *pDevice, Event *pEvent){
		char command[255];
		char filename[255];
		float x = 0, y = 0;
		MATCH(command, "ChangeBackground"){
			sprintf(command, "%s %s", command, filename);
			Load(pDevice, filename, 0, 0);
			return TRUE;
		}
		return FALSE;
	}
	void Update(){
		// Sets the Scroll Direction for the correct
		// Drawing mechanism
		if(m_xVelocity == 0 && m_yVelocity == 0)
				m_Direction = NONE;
		if(m_xVelocity != 0 && m_yVelocity == 0)
				m_Direction = HORIZONTAL;
		if(m_xVelocity == 0 && m_yVelocity != 0)
				m_Direction = VERTICAL;
		if(m_xVelocity != 0 && m_yVelocity != 0)
				m_Direction = BOTH;
		// Updates Position
		m_xPosition += m_xVelocity;
		m_yPosition += m_yVelocity;
	}
	void Frame(IDirect3DDevice9 *pDevice){
		Update();
		m_xOffset = (m_xPosition < 0) ? m_pPanel.getWidth() : -m_pPanel.getWidth();
		m_xPosition = (m_xPosition < -m_pPanel.getWidth() || 
			m_xPosition > m_pPanel.getWidth()) ? 0 : m_xPosition;
		m_yOffset = (m_yPosition < 0) ? m_pPanel.getHeight() : -m_pPanel.getHeight();
		m_yPosition = (m_yPosition < - m_pPanel.getHeight() || 
			m_yPosition > m_pPanel.getHeight()) ? 0 : m_yPosition;	
		switch(m_Direction){
			case NONE:
				m_pPanel.TranslateAbs(m_xPosition,0);
				m_pPanel.Render(pDevice);
			return;
			case HORIZONTAL:
				m_xOffset+= m_xPosition;
				m_pPanel.TranslateAbs(m_xPosition,0);
				m_pPanel.Render(pDevice);
				m_pPanel.TranslateAbs(m_xOffset, 0);
				m_pPanel.Render(pDevice);
			return;
			case VERTICAL:
				m_yOffset+= m_yPosition;
				m_pPanel.TranslateAbs(0, m_yPosition);
				m_pPanel.Render(pDevice);
				m_pPanel.TranslateAbs(0, m_yOffset);
				m_pPanel.Render(pDevice);
			return;
			case BOTH: 
			break;
		}
	}
protected:
	float		m_xOffset,   m_yOffset;
	float		m_xPosition, m_yPosition; 
	float		m_xVelocity, m_yVelocity;
	MOVEMENT	m_Direction;
	CPanelEx	m_pPanel;
};
/*

class CBackground	:	public CSprite{
protected:
	typedef enum { NONE = 0, HORIZONTAL = 1, VERTICAL = 2, BOTH = 3} MOVEMENT;
	MOVEMENT	m_Direction;
	float		m_xOffset; 
	float		m_yOffset;
	float		m_xVelocity;
	float		m_yVelocity;
	D3DXVECTOR3 m_vPosition;
	RECT		m_rcRect;
public:
	CBackground(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,	
				char *pImgFilename, D3DCOLOR color = NULL);
	~CBackground();
	HRESULT Load(char *pImgFilename, D3DCOLOR color = NULL);
	void setDirectionAbs(int x, int y);
	void Frame();
	void Render();
	void Update();
};
*/
/*
class CBackgroundParseable : public CBackground, CParseAble{
public:
	CBackgroundParseable(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,	
				  char *pImgFilename, D3DCOLOR color = NULL);
	~CBackgroundParseable();
	BOOL ParserCallback(char *pString, char *pOutput = NULL);
	char *toString(){
		return "CBackground";
	}
};
*/
#endif