#pragma once
#include "./Core_Graphics.h"
#include "./CSprite.h"
#include "./CFont.h"
class UIPanel;
class UIElement{ // ELEMENT IS A SINGLE CLICK ABLE OBJECT
public:
	virtual void Render() = 0;
	virtual void OnMouseOver() = 0;
	virtual void OnMouseOut() = 0;
	virtual void OnMouseLeftClick() = 0;
	virtual void OnMouseRightClick() = 0;
	virtual RECT GetRect() = 0;
};
typedef void (*Function)(void *Ptr, long Purpose);
class SpriteElement : public UIElement{
public:
	SpriteElement(UIPanel *pUIPanel, char *pTextureString, int x, int y, char *pScriptCommand);
	SpriteElement(UIPanel *pUIPanel, CSpriteDx *pSprite, int x, int y, char *pScriptCommand);
	void Render();
	void OnMouseOver(){	m_pSprite->SetModulateColorAbs(255, 255,0,0);}
	void OnMouseOut(){	m_pSprite->SetModulateColorAbs(255, 255,255,255);}
	void OnMouseLeftClick();
	void OnMouseRightClick(){}
	RECT GetRect(){ return m_pSprite->GetRect();}
protected:	
	char *m_pScriptCommand;
	CSpriteDx *m_pSprite;
	UIPanel *m_pUIPanel;
};
class TextElement : public UIElement{
public:
	TextElement(UIPanel *pUIPanel, char *pTextString, int x, int y, char *pScriptCommand);
	RECT GetRect();
	void OnMouseOver(){	m_pFont->SetColorAbs(255, 0, 0,255); }
	void OnMouseOut(){	m_pFont->SetColorAbs(255, 255, 255, 255);	}
	void OnMouseLeftClick();
	void OnMouseRightClick(){	}
	void Render();
protected:	
	char *m_pScriptCommand;
	CFontEx *m_pFont;
	UIPanel *m_pUIPanel;
};

class UIPanel{
public:
	UIPanel(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, float Offx, float Offy, 
		PFNCMDPARSERCALLBACK pfnCommandParserCallback = NULL);
	void AddTextElement(char *pTextString, int x, int y, char *pScriptCommand);
	void AddSpriteElement(char *pTextureString, int x, int y, char *pScriptCommand);
	void AddAnimatedSpriteElement(char *pTextureString, int x, int y, char *pScriptCommand);
	void RenderElements();
	void TrackMouseInputProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	POINT m_MousePoint;
	vector<UIElement*> m_pUIElements;
	IDirect3DDevice9 *m_pDevice;
	ID3DXSprite *m_pSprite;
	float m_fontsize;
	float m_fOffx, m_fOffy;
	PFNCMDPARSERCALLBACK m_pfnCommandParserCallback;
};
UIPanel::UIPanel(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, float Offx, float Offy, 
				 PFNCMDPARSERCALLBACK pfnCommandParserCallback){
	m_pDevice = pDevice;
	m_pSprite = pSprite;
	m_fontsize = 60;
	m_fOffx = Offx;
	m_fOffy = Offy;
	m_MousePoint.x = m_MousePoint.y = 0;
	m_pfnCommandParserCallback = pfnCommandParserCallback;
}
void UIPanel::AddTextElement(char *pTextString, int x, int y, char *pScriptCommand){
	TextElement *pTextElement = new TextElement(this, pTextString, m_fOffx+x, m_fOffy+y, pScriptCommand);
	m_pUIElements.push_back(pTextElement);
}
void UIPanel::AddSpriteElement(char *pTextureString, int x, int y, char *pScriptCommand){
	SpriteElement *pSpriteElement = new SpriteElement(this, pTextureString, m_fOffx+x, m_fOffy+y, pScriptCommand);
	m_pUIElements.push_back(pSpriteElement);
}
void UIPanel::AddAnimatedSpriteElement(char *pTextureString, int x, int y, char *pScriptCommand){
	//CAnimSprite ce = new CAnimSprite(pTexture, pTextureString, 0);
	//SpriteElement *pSpriteElement = new SpriteElement(this, , m_fOffx+x, m_fOffy+y, pScriptCommand);
	//m_pUIElements.push_back(pSpriteElement);
}
void UIPanel::RenderElements(){
	for(int i = 0; i < m_pUIElements.size(); i++){
		if(PtInRect(&m_pUIElements[i]->GetRect(), m_MousePoint))		
			m_pUIElements[i]->OnMouseOver();
		else	
			m_pUIElements[i]->OnMouseOut();
		m_pUIElements[i]->Render();
	}
}
void UIPanel::TrackMouseInputProc(UINT uMsg, WPARAM wParam, LPARAM lParam){
		switch(uMsg){
			case WM_MOUSEMOVE:
				m_MousePoint.x = LOWORD(lParam);
				m_MousePoint.y = HIWORD(lParam);
		break;
		case WM_LBUTTONDOWN:
			for(int i = 0; i < m_pUIElements.size(); i++){
				if(::PtInRect(&m_pUIElements[i]->GetRect(), m_MousePoint))
					m_pUIElements[i]->OnMouseLeftClick();
			}
		break;
	}
}

TextElement::TextElement(UIPanel *pUIPanel, char *pTextString, int x, int y, char *pScriptCommand) : m_pUIPanel(pUIPanel){
	m_pFont = new CFontEx(m_pUIPanel->m_pDevice, m_pUIPanel->m_pSprite, "Arial", m_pUIPanel->m_fontsize);
	m_pFont->SetTranslationAbs(x, y);
	m_pFont->SetText(pTextString);
	m_pScriptCommand = pScriptCommand;
}
RECT TextElement::GetRect(){ return m_pFont->GetRect(m_pUIPanel->m_pSprite); }
void TextElement::Render(){	m_pFont->Render(m_pUIPanel->m_pDevice, m_pUIPanel->m_pSprite); }
void TextElement::OnMouseLeftClick(){ 
	if(m_pUIPanel->m_pfnCommandParserCallback != NULL)
		m_pUIPanel->m_pfnCommandParserCallback(m_pScriptCommand);
}
SpriteElement::SpriteElement(UIPanel *pUIPanel, char *pTextureString, int x, int y, char *pScriptCommand){
	m_pUIPanel = pUIPanel;
	m_pSprite = new CSpriteDx(pUIPanel->m_pDevice, pTextureString);
	m_pSprite->SetTranslationAbs(x, y);
	m_pScriptCommand = pScriptCommand;
}
SpriteElement::SpriteElement(UIPanel *pUIPanel, CSpriteDx *pSprite, int x, int y, char *pScriptCommand){
	m_pUIPanel = pUIPanel;
	m_pSprite = pSprite;
	m_pSprite->SetTranslationAbs(x, y);
	m_pScriptCommand = pScriptCommand;
}
void SpriteElement::Render(){ 
	m_pSprite->Render(m_pUIPanel->m_pDevice, m_pUIPanel->m_pSprite);
}
void SpriteElement::OnMouseLeftClick(){
	if(m_pUIPanel->m_pfnCommandParserCallback != NULL)
		m_pUIPanel->m_pfnCommandParserCallback(m_pScriptCommand); 
}