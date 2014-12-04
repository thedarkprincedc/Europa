/****************************************************
 Brett Mosley - CFont Wrapper
 Wraps D3DX2D Font Class, and takes a sprite object 
		to speed up rendering
 DT_CALCRECT is used to calculation the rectangle 
		size of text
 Notes: This class only takes a pointer to a text string
		it does not allocate text string anymore because
		it was inflexable and not efficient
***************************************************/
#ifndef _CFONTEX_H_
#define _CFONTEX_H_

class CFontEx : public CRenderObject{
public:
	virtual RITTI getRITTI(){	return FONT;	}
	CFontEx(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,
		  char *pfontFace = "Arial", float fontSize = 12, 
		  D3DCOLOR color = D3DCOLOR_ARGB(255,255,255,255));
	virtual ~CFontEx();
	void Init(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite);	
	void SetFont(char *text);					  // Set Font Face
	int GetFontSize();
	void SetSize(int size);						  // Set Font Size
	void SetColor(D3DCOLOR color);				  // Set Font Color
	void SetColor(D3DCOLOR *color){	m_fontColor = *color;	}
	virtual void SetText(char *pText);			  // Set Text
	void GetText(char *pText);					  // Get Text
	void operator = (char *pText);				  // = Operator
	const char *GetText(){	return m_pTextString.c_str();	}    
	RECT *GetSize();							  // Get the Size of the text
	RECT getRect();								  // Get the text RECT
	float getX();
	float getY();
	float getWidth();
	float getHeight();
	virtual void Render();						  // Renders the text
	void SetTranslation(D3DXVECTOR2 translation); // Moves text in x, y Axises
	inline void SetTranslation(float x, float y); // Moves text in x, y Axises
	inline void SetTranslationRel(float x, float y){ // Moves text in x, y Axises
		m_vTranslation.x += x;
		m_vTranslation.y += y;
	}
	void GetTranslation(D3DXVECTOR2 *translation);// Gets the Axies of the Text
	void SetScale(D3DXVECTOR2 scale);			  // Sets the Scale of the text
	void GetScale(D3DXVECTOR2 *scale);			  // Gets the Scale of the text
	void SetRotation(float rotation);			  // Sets the Rotation of the text
	void GetRotation(float *rotation);			  // Gets the Rotation of the text
	inline HRESULT CreateFont();				  // Creates the font from member variables
	void Begin(){	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);	}
	void End(){		m_pSprite->End();	}
protected:
	IDirect3DDevice9			 *m_pDevice;	  // D3D Device Object
	ID3DXSprite					 *m_pSprite;	  // Sprite Object
	ID3DXFont					 *m_pFont;		  // Font Object
	string						 m_pTextString;  // Pointer TextString
	char						 *m_fontFace;	  // Font Face
	float						  m_fontSize;	  // Font Size
	D3DCOLOR					  m_fontColor;	  // Font Color
	RECT						  m_rcfontSize;	  // Font Size(use GetSize())
	RECT						  m_rcfontRect;	  // Font Rect (used to set the 
												  // size of the rendering rect)
	D3DXVECTOR2					  m_vScale;		  // Scale
	D3DXVECTOR2					  m_vTranslation; // Translation
	float						  m_fRotation;	  // Rotation (In Radians)
	BOOL						  m_bSizeChanged; // BOOL Size Change
};
#endif