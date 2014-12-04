#include <vector>
#include "cdxcamera.h"
using namespace std;
class CMesh{
public:	
	CMesh(IDirect3DDevice9* pDevice);
	CMesh(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath = "./");
	HRESULT LoadMesh(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath = "./");
	//void SetScaleRel(float x, float y, float z);
	D3DXVECTOR3 minBounds, maxBounds;
	void CalculateBoundingColisionBox(){	
		BYTE* verts = NULL;
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (LPVOID*)&verts);
		D3DXComputeBoundingBox((D3DXVECTOR3*)verts, m_pMesh->GetNumVertices(), 
			D3DXGetFVFVertexSize(m_pMesh->GetFVF()), &minBounds, &maxBounds); 
		m_pMesh->UnlockVertexBuffer();
	}
	void SetTranslationAbs(float x, float y, float z){
		m_vtranslate.x = x;	m_vtranslate.y = y;	m_vtranslate.z = z;
		m_translate._41 = x;	m_translate._42 = y;	m_translate._43 = z;
	}
	void SetTranslationRel(float x, float y, float z){
		m_vtranslate.x += x;	m_vtranslate.y += y;	m_vtranslate.z += z;
		m_translate._41 += x;	m_translate._42 += y;	m_translate._43 += z;
	}
	void RotateRel(float x, float y, float z){
		m_vrotation.x += x;	m_vrotation.y += y;	m_vrotation.z += z;
		D3DXMatrixRotationYawPitchRoll(&m_rotation, m_vrotation.x, m_vrotation.y, m_vrotation.z);
	}
	void SetScaleRel(float x, float y, float z){
		::D3DXMatrixScaling(&m_scale, x , y, z);
		//m_vScale.x += x;		m_vScale.y += y;
		//m_scale._11 += x;		m_scale._22 += y;	m_scale._33 += z;
	}
	void Render(IDirect3DDevice9* pDevice);
	ID3DXMesh *GetPtrToMesh(){ return m_pMesh; }
protected:
	ID3DXMesh*				   m_pMesh;
	vector<D3DMATERIAL9>	   m_vMaterials;
	vector<LPDIRECT3DTEXTURE9> m_vTextures;
	D3DXVECTOR3				   m_vtranslate;
	D3DXVECTOR3				   m_vrotation;
	D3DXMATRIX				   m_scale;
	D3DXMATRIX				   m_translate;
	D3DXMATRIX				   m_rotation;
	D3DXMATRIX				   m_transform;
};
CMesh::CMesh(IDirect3DDevice9* pDevice){

}
CMesh::CMesh(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath){
	D3DXMatrixIdentity(&m_transform);
	D3DXMatrixIdentity(&m_scale);
	D3DXMatrixIdentity(&m_translate);
	D3DXMatrixIdentity(&m_rotation);
	m_vrotation = D3DXVECTOR3(0,0,0);
	LoadMesh(pDevice, pszFilename, pszTexturepath);
}
HRESULT CMesh::LoadMesh(IDirect3DDevice9* pDevice, char *pszFileName, char *pszTexturePath){
	ID3DXBuffer			*adjBuffer  = 0;
	ID3DXBuffer			*mtrlBuffer = 0;
	DWORD				numMtrls   = 0;
	IDirect3DTexture9	*tex = NULL;
	if(FAILED(D3DXLoadMeshFromX(pszFileName, D3DXMESH_MANAGED, 
								pDevice, &adjBuffer, 
								&mtrlBuffer, 0, &numMtrls, 
								&m_pMesh))){
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}
	if(mtrlBuffer != 0 && numMtrls != 0){
		D3DXMATERIAL *mtrls = (D3DXMATERIAL*) mtrlBuffer->GetBufferPointer();
		for(int i = 0; i < numMtrls; i++){
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_vMaterials.push_back(mtrls[i].MatD3D);
			if(mtrls[i].pTextureFilename != 0){
				tex = NULL;
				char texName[64];
				strcpy(texName,pszTexturePath);
				strcat(texName,mtrls[i].pTextureFilename);
				D3DXCreateTextureFromFile(pDevice, texName, &tex);
				m_vTextures.push_back(tex);
			}
			else{
				m_vTextures.push_back(0);
			}
		}
	}
	mtrlBuffer->Release();
	if(FAILED(m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|
								 D3DXMESHOPT_COMPACT|
								 D3DXMESHOPT_VERTEXCACHE,
								 (DWORD*)adjBuffer->GetBufferPointer(),
								 0, 0, 0))){
		MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}
	adjBuffer->Release();
	return S_OK;
}
void CMesh::Render(IDirect3DDevice9* pDevice){
	m_transform = m_scale * m_rotation * m_translate;
	pDevice->SetTransform(D3DTS_WORLD, &m_transform);
	for(int i = 0; i < m_vMaterials.size(); i++){
		pDevice->SetMaterial( &m_vMaterials[i] );
		pDevice->SetTexture(0, m_vTextures[i]);
		m_pMesh->DrawSubset(i);
	}
	D3DXMatrixIdentity(&m_transform);
	pDevice->SetTransform(D3DTS_WORLD, &m_transform);
}
class CDXMesh{
public:
	CDXMesh(IDirect3DDevice9 *ppDevice);
	HRESULT LoadMesh(char *pszFileName, char *pszTexturePath = "./");
	BOOL Render(float b);
	HRESULT Shutdown();
	IDirect3DDevice9				*m_pDevice;
	ID3DXMesh						*m_pMesh;
	vector<D3DMATERIAL9>			m_pMaterials;
	vector<LPDIRECT3DTEXTURE9>		m_pTextures;
};

CDXMesh::CDXMesh(IDirect3DDevice9 *ppDevice)
	:	m_pDevice(ppDevice), m_pMesh(NULL){}
HRESULT CDXMesh::LoadMesh(char *pszFileName, char *pszTexturePath){
	HRESULT				r;
	ID3DXBuffer			*adjBuffer  = 0;
	ID3DXBuffer			*mtrlBuffer = 0;
	DWORD				numMtrls   = 0;
	IDirect3DTexture9	*tex = NULL;
	if(FAILED(D3DXLoadMeshFromX(pszFileName, D3DXMESH_MANAGED, 
								m_pDevice, &adjBuffer, 
								&mtrlBuffer, 0, &numMtrls, 
								&m_pMesh))){
		::MessageBox(0, "D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
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
	if(FAILED(m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|
								 D3DXMESHOPT_COMPACT|
								 D3DXMESHOPT_VERTEXCACHE,
								 (DWORD*)adjBuffer->GetBufferPointer(),
								 0, 0, 0))){
		MessageBox(0, "OptimizeInplace() - FAILED", 0, 0);
		return false;
	}
	adjBuffer->Release();
	return S_OK;
}
BOOL CDXMesh::Render(float b){
	if( m_pDevice ){	
		for(int i = 0; i < m_pMaterials.size(); i++){
			m_pDevice->SetMaterial( &m_pMaterials[i] );
			m_pDevice->SetTexture(0, m_pTextures[i]);
			m_pMesh->DrawSubset(i);
		}	
	}
	return TRUE;
}
HRESULT CDXMesh::Shutdown(){
	return S_OK;
}