#include "./Global_Framework.h"
CFontEx::CFontEx(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,
		char *pfontFace, float fontSize, D3DCOLOR color)
	:	m_pFont(NULL), m_fontFace(pfontFace), 
		m_fontSize(fontSize), m_fontColor(color){
	Init(pDevice, pSprite);
}
CFontEx::~CFontEx(){
	delete [] m_fontFace;
}
void CFontEx::Init(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
	m_pDevice = pDevice;
	m_pSprite = pSprite;
	ZeroMemory(&m_rcfontSize, sizeof(RECT));
	ZeroMemory(&m_rcfontRect, sizeof(RECT));
	m_vScale		=	D3DXVECTOR2(1.0,1.0);	//	Init Scale
	m_vTranslation	=	D3DXVECTOR2(0,0);		//	Init Translation
	m_fRotation		=	0.0f;					//	Init Rotation
	m_pTextString	=	"";
	m_bSizeChanged	=	FALSE;
	CreateFont();
}
void CFontEx::SetFont(char *font){
	m_fontFace = font;		// Sets the font face to the val of font
	CreateFont();			// Creates a New Font
	m_bSizeChanged = TRUE;	// Sets the Size has Changed BOOLEAN to TRUE
}
void CFontEx::SetSize(int size){
	m_fontSize = size;		// Set Font Size
	CreateFont();			// Creates a New Font
	m_bSizeChanged = TRUE;	// Sets the Size has Changed BOOLEAN to TRUE
}
void CFontEx::SetColor(D3DCOLOR color){
	m_fontColor = color;	// Set Font
}
void CFontEx::SetText(char *pText){
	m_pTextString = pText;
	m_bSizeChanged = TRUE;
}
void CFontEx::operator=(char *pText){
	m_pTextString = pText;
	m_bSizeChanged = TRUE;
}
void CFontEx::GetText(char *pText){
	//if(m_pTextString == NULL)	// Check if m_pText is NULL
	//	pText = NULL;	// Set pText to NULL
    pText = (char*)m_pTextString.c_str();	// Get m_pText
}
HRESULT CFontEx::CreateFont(){
	if(FAILED(D3DXCreateFont(m_pDevice, m_fontSize, 0, FW_BOLD, 0, FALSE, 
	   DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 
	   DEFAULT_PITCH | FF_DONTCARE, TEXT(m_fontFace), &m_pFont))){
		 ::OutputDebugString("D3DXCreateFont(): FAILED");
		return E_FAIL;
	}
	return S_OK;
}
RECT *CFontEx::GetSize(){
	//	This Function returns the size of the rectangle
	//	to hold the text it doesnt render anything
	ZeroMemory(&m_rcfontSize, sizeof(RECT));
	if(FAILED(m_pFont->DrawText(m_pSprite, m_pTextString.c_str(), 
		-1, &m_rcfontSize, DT_CALCRECT, m_fontColor))){
		::OutputDebugString("FontSize(): FAILED");
		return NULL;
	}
	return &m_rcfontSize;
}
RECT CFontEx::getRect(){
	ZeroMemory(&m_rcfontSize, sizeof(RECT));
	if(FAILED(m_pFont->DrawText(m_pSprite, m_pTextString.c_str(), 
		-1, &m_rcfontSize, DT_CALCRECT, m_fontColor))){
		::OutputDebugString("FontSize(): FAILED");
	}
	m_rcfontSize.top	+= static_cast<LONG>(m_vTranslation.y);
	m_rcfontSize.left	+= static_cast<LONG>(m_vTranslation.x);
	m_rcfontSize.right	+= static_cast<LONG>(m_vTranslation.x);
	m_rcfontSize.bottom += static_cast<LONG>(m_vTranslation.y);
	return m_rcfontSize;
}
void CFontEx::Render(){
	D3DXMATRIX mat, matScale, matRotaZ, matTrans, OriginalMat;
	D3DXMatrixIdentity(&mat);							// Gets Identity Matricies
	D3DXMatrixIdentity(&OriginalMat);
	D3DXMatrixScaling(&matScale, m_vScale.x,			// Sets Scaling Matrix D3DXVECTOR2
					  m_vScale.y, 1.0f); 				// and NULLs the Z-AXIS
	D3DXMatrixRotationZ(&matRotaZ, m_fRotation);		// Sets the Rotatation Matrix 
														// with a float
	D3DXMatrixTranslation(&matTrans,  m_vTranslation.x, // Sets Translation Matrix
						  m_vTranslation.y, 0.0f);      // with D3DXVECTOR2 and 
														// NULLs the Z-AXIS
	D3DXMatrixMultiply(&mat, &mat, &matRotaZ);			// Multiplies Rotation (Z-AXIS)
	D3DXMatrixMultiply(&mat, &mat, &matTrans);			// Multiplies Translation
	D3DXMatrixMultiply(&mat, &mat, &matScale);			// Multiplies Scale
	m_pSprite->SetTransform(&mat);						// Sets Transformation Matrix
	/******************Drawing Code*****************/
	if(m_rcfontRect.top!=0|| m_rcfontRect.left!= 0||	// If FontRect is (not zero) then 
	   m_rcfontRect.bottom!= 0 ||m_rcfontRect.right != 0) 
	// draw font with fontRect Specifying the size 
		m_pFont->DrawText(m_pSprite, m_pTextString.c_str(), -1, &m_rcfontRect, 0, m_fontColor );
	// Draw the whole font
	else	m_pFont->DrawText(m_pSprite, m_pTextString.c_str(), -1, NULL, 0, m_fontColor );
	/***********************************************/
	m_pSprite->SetTransform(&OriginalMat);	// Set Transformation Matricies 
											// back to original values
}
void CFontEx::SetTranslation(D3DXVECTOR2 translation){
	m_vTranslation = translation;
}
void CFontEx::SetTranslation(float x, float y){ // Moves text in x, y Axises
	m_vTranslation.x = x;
	m_vTranslation.y = y;
}
void CFontEx::GetTranslation(D3DXVECTOR2 *translation){
	*translation = m_vTranslation;
}
void CFontEx::SetScale(D3DXVECTOR2 scale){	m_vScale = scale;			}
void CFontEx::GetScale(D3DXVECTOR2 *scale){	*scale = m_vScale;			}
void CFontEx::SetRotation(float rotation){	m_fRotation = rotation;		}
void CFontEx::GetRotation(float *rotation){	*rotation = m_fRotation;	}
float CFontEx::getX(){ return this->m_vTranslation.x; }
float CFontEx::getY(){ return this->m_vTranslation.y; }
float CFontEx::getWidth(){ return m_rcfontSize.right; }
float CFontEx::getHeight(){ return m_rcfontSize.bottom; }
int CFontEx::GetFontSize(){ return m_fontSize; }