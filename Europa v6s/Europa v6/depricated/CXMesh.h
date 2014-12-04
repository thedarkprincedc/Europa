/*******************************************
	Very basic Mesh class i wrote loads one
	mesh at a time until i fix the see though
	problem i wont worry about it.
BrettMosley@hotmail.com
*******************************************/
#ifndef _CXMESH_H_
#define _CXMESH_H_
#include <d3d9.h>
#include <d3dx9.h>
#include "CWorldTransform.h"
#include <vector>
using namespace std;

class CXMesh : public CWorldTransform{
public:
	//CXMesh(){
	//}
	CXMesh(IDirect3DDevice9 *pDevice);
	CXMesh(IDirect3DDevice9 *pDevice, char *pszMeshFileName, 
		char *pszTexturePath = "./");
	virtual ~CXMesh();
	HRESULT LoadMesh(char *pszMeshFileName, char *pszTexturePath = "./");
	BOOL Render();
	HRESULT Shutdown();
	ID3DXMesh *GetD3DMesh(){ return m_pMesh; }
private:
	IDirect3DDevice9				*m_pDevice;
	ID3DXMesh						*m_pMesh;
	vector<D3DMATERIAL9>			m_pMaterials;
	vector<LPDIRECT3DTEXTURE9>		m_pTextures;
};
#endif