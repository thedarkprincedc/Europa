#include "./CFont.h"


/*	
RECT *CFontEx::GetSize(){
	//	This Function returns the size of the rectangle
	//	to hold the text it doesnt render anything
	WCHAR wchTextString[255];	//unicode char
	mbstowcs(wchTextString, m_pTextString.c_str(), 255); // conversion

	ZeroMemory(&m_rcfontSize, sizeof(RECT));
	//if(FAILED(m_pFont->DrawText(m_pSprite, wchTextString, 
	//	-1, &m_rcfontSize, DT_CALCRECT, m_fontColor))){
	//	::OutputDebugString(L"FontSize(): FAILED");
	//	return NULL;
	//}
	return &m_rcfontSize;
}
RECT CFontEx::getRect(){
	ZeroMemory(&m_rcfontSize, sizeof(RECT));
	WCHAR wchTextString[255];	//unicode char
	mbstowcs(wchTextString, m_pTextString.c_str(), 255); // conversion

	//if(FAILED(m_pFont->DrawText(m_pSprite, wchTextString, 
	//	-1, &m_rcfontSize, DT_CALCRECT, m_fontColor))){
	//	::OutputDebugString(L"FontSize(): FAILED");
	//}
	m_rcfontSize.top	+= static_cast<LONG>(m_vTranslation.y);
	m_rcfontSize.left	+= static_cast<LONG>(m_vTranslation.x);
	m_rcfontSize.right	+= static_cast<LONG>(m_vTranslation.x);
	m_rcfontSize.bottom += static_cast<LONG>(m_vTranslation.y);
	return m_rcfontSize;
}
void CFontEx::Render(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite){
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
	pSprite->SetTransform(&mat);						// Sets Transformation Matrix
		WCHAR wchTextString[255];	//unicode char
	mbstowcs(wchTextString, m_pTextString.c_str(), 255); // conversion
	
	if(m_rcfontRect.top!=0|| m_rcfontRect.left!= 0||	// If FontRect is (not zero) then 
	   m_rcfontRect.bottom!= 0 ||m_rcfontRect.right != 0) 
	// draw font with fontRect Specifying the size 
		m_pFont->DrawText(pSprite, wchTextString, -1, &m_rcfontRect, 0, m_fontColor );
	// Draw the whole font
	else	m_pFont->DrawText(pSprite, wchTextString, -1, NULL, 0, m_fontColor );

	pSprite->SetTransform(&OriginalMat);	// Set Transformation Matricies 
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
*/