#pragma once
#include "../PHNX ENGINE/phoenixengine.h"
/*
class CMappedFont
{
public:
	CMappedFont(void);
	CMappedFont(IDirect3DDevice9 *pDevice, char *pszFontMapFilepath, 
		int CharsPerLine, int LetterWidth, int LetterHeight)
		:	m_nCharPerLine(CharsPerLine), m_fLetterWidth(LetterWidth), 
			m_fLetterHeight(LetterHeight){
		LoadFontMap(pDevice, pszFontMapFilepath, NULL);
	}
	HRESULT LoadFontMap(IDirect3DDevice9 *pDevice, char *pszFontMapFilepath, 
					 D3DCOLOR d3dcolor){
		// Create a Texture from file
		if(FAILED(D3DXCreateTextureFromFileEx(pDevice, pszFontMapFilepath, 
				  0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
				  D3DX_DEFAULT, d3dcolor, NULL, NULL, &m_pTexture))){
			::OutputDebugString("Could not create Texture from file: ");
			::OutputDebugString(pszFontMapFilepath);
			return E_FAIL;
		}
		return S_OK;
	}
	HRESULT Render(IDirect3DDevice9 *pDevice){
		return S_OK;
	}
	HRESULT RenderSingleChar(){}
	SetText(char *pText);
	GetText(char *pText);
	SetCharsPerLine();
	GetCharsPerLine()
	GetLetterHeight();
	SetLetterHeight();
	GetLetterWidth();
	SetLetterWidth();
public:
	virtual ~CMappedFont(void);
protected:
	IDirect3DTexture9 *m_pTexture;
private:
	string			m_strText;
	int				m_nCharPerLine;
	float			m_fLetterWidth;
	float			m_fLetterHeight;
	D3DSURFACE_DESC m_TextureDesc;
};
*/