#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
using namespace std;

class CMesh{
public:	
	CMesh(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath = "./");
	HRESULT LoadMesh(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath = "./");
	void Render(IDirect3DDevice9* pDevice, 
		D3DXVECTOR3 position = D3DXVECTOR3(0,0,0), 
		D3DXVECTOR3 rotation = D3DXVECTOR3(0,0,0), 
		D3DXVECTOR3 scale = D3DXVECTOR3(0,0,0));
	void Render(IDirect3DDevice9* pDevice, D3DXMATRIX *transMatrix);
protected:
	ID3DXMesh*				   m_pMesh;			 
	vector<D3DMATERIAL9>	   m_vMaterials;	 
	vector<LPDIRECT3DTEXTURE9> m_vTextures;
};
