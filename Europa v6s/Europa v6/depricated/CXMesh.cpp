#include "CXMesh.h"
CXMesh::CXMesh(IDirect3DDevice9 *pDevice)
:	m_pDevice(pDevice), m_pMesh(NULL){
}
CXMesh::CXMesh(IDirect3DDevice9 *pDevice, char *pszMeshFileName, 
	   char *pszTexturePath) : m_pDevice(pDevice){
	LoadMesh(pszMeshFileName, pszTexturePath);
}
CXMesh::~CXMesh(){  
}
HRESULT CXMesh::LoadMesh(char *pszMeshFileName, char *pszTexturePath){
	ID3DXBuffer			*adjBuffer  = 0; // adjacent buffer
	ID3DXBuffer			*mtrlBuffer = 0; // material buffer
	DWORD				numMtrls   = 0;	 // number of materials
	IDirect3DTexture9	*tex = NULL;	 // pointer to texture
	if(FAILED(D3DXLoadMeshFromX(pszMeshFileName, D3DXMESH_MANAGED, 
		m_pDevice, &adjBuffer, &mtrlBuffer, 0, &numMtrls, &m_pMesh))){
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return E_FAIL;
	}
	if(mtrlBuffer != 0 && numMtrls != 0){
		D3DXMATERIAL *mtrls = (D3DXMATERIAL*) mtrlBuffer->GetBufferPointer();
		for(int i = 0; i < numMtrls; i++){
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_pMaterials.push_back(mtrls[i].MatD3D);
			if(mtrls[i].pTextureFilename != 0){
				tex = NULL;
				char texName[64];
				strcpy(texName,pszTexturePath);
				strcat(texName,mtrls[i].pTextureFilename);
				D3DXCreateTextureFromFile(m_pDevice, texName, &tex);
				m_pTextures.push_back(tex);
			}
			else{
				m_pTextures.push_back(0);
			}
		}
	}
	mtrlBuffer->Release();
	if(FAILED(m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT |
			  D3DXMESHOPT_COMPACT | D3DXMESHOPT_VERTEXCACHE,
			  (DWORD*)adjBuffer->GetBufferPointer(), 0, 0, 0))){
		::MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return E_FAIL;
	}
	adjBuffer->Release();
	return S_OK;
}

BOOL CXMesh::Render(){
	if( m_pDevice ){	
		m_pDevice->SetTransform(D3DTS_WORLD, GetTransform());
		//m_pDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );  
		//m_pDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
		//m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME );      
		for(int i = 0; i < m_pMaterials.size(); i++){
			m_pDevice->SetMaterial( &m_pMaterials[i] );
			m_pDevice->SetTexture(0, m_pTextures[i]);
			m_pMesh->DrawSubset(i);
		}	
	}
	return TRUE;
}

HRESULT CXMesh::Shutdown(){	return S_OK; }