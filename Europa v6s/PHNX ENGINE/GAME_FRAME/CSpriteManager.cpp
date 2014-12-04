#include "./Global_Framework.h"
CSpriteManager *CSpriteManager::m_pSpriteManagerSingleton = NULL;
IDirect3DTexture9 *CSpriteManager::getSprite(char *filename, IDirect3DDevice9 *pDevice, 
									D3DCOLOR colorKey){
	if((Iter = m_TextureCache.find(filename)) != m_TextureCache.end())
		return (*Iter).second;
	IDirect3DTexture9 *pTempTexture = NULL;
	if(FAILED(D3DXCreateTextureFromFileEx(pDevice, filename, D3DX_DEFAULT, 
		  D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		  D3DX_DEFAULT, D3DX_DEFAULT, colorKey, NULL, NULL, &pTempTexture))){
		::OutputDebugString("Could not create a texture from the following file:\n");
		::OutputDebugString("filename");
		::OutputDebugString("\n");
		return NULL;	
	}
	m_TextureCache[filename] = pTempTexture;
	return pTempTexture;
}