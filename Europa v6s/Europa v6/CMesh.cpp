#include "./CMesh.h"
CMesh::CMesh(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath)
{
	LoadMesh(pDevice, pszFilename, pszTexturepath);
}
HRESULT CMesh::LoadMesh(IDirect3DDevice9* pDevice, char *pszFileName, char *pszTexturePath)
{
	ID3DXBuffer			*adjBuffer  = 0;
	ID3DXBuffer			*mtrlBuffer = 0;
	DWORD				numMtrls   = 0;
	IDirect3DTexture9	*tex = NULL;
	WCHAR wchfilename[255];	//unicode char
	mbstowcs(wchfilename, pszFileName, 255); // conversion
	if(FAILED(D3DXLoadMeshFromX(wchfilename, D3DXMESH_MANAGED, pDevice, &adjBuffer, 
								&mtrlBuffer, 0, &numMtrls, &m_pMesh))){
		::MessageBox(0, L"D3DXLoadMeshFromX() - FAILED", 0, 0);
		return false;
	}
	if(mtrlBuffer != 0 && numMtrls != 0)
	{
		D3DXMATERIAL *mtrls = (D3DXMATERIAL*) mtrlBuffer->GetBufferPointer();
		for(int i = 0; i < numMtrls; i++)
		{
			mtrls[i].MatD3D.Ambient = mtrls[i].MatD3D.Diffuse;
			m_vMaterials.push_back(mtrls[i].MatD3D);
			if(mtrls[i].pTextureFilename != 0)
			{
				tex = NULL;
				char texName[64];
				strcpy(texName,pszTexturePath);
				if(strlen(texName) > 2)
					strcat(texName, "/");
				strcat(texName,mtrls[i].pTextureFilename);
				mbstowcs(wchfilename, texName, 255); // conversion
				D3DXCreateTextureFromFile(pDevice, wchfilename, &tex);
				m_vTextures.push_back(tex);
			}
			else
				m_vTextures.push_back(0);
		}
	}
	mtrlBuffer->Release();
	if(FAILED(m_pMesh->OptimizeInplace(D3DXMESHOPT_ATTRSORT|D3DXMESHOPT_COMPACT|D3DXMESHOPT_VERTEXCACHE,
								 (DWORD*)adjBuffer->GetBufferPointer(),0, 0, 0)))
	{
		MessageBox(0, L"OptimizeInplace() - FAILED", 0, 0);
		return false;
	}
	adjBuffer->Release();
	return S_OK;
}
void CMesh::Render(IDirect3DDevice9* pDevice, D3DXMATRIX *transMatrix)
{
	pDevice->SetTransform(D3DTS_WORLD, transMatrix);
	for(int i = 0; i < m_vMaterials.size(); i++)
	{
		pDevice->SetMaterial( &m_vMaterials[i] );
		pDevice->SetTexture(0, m_vTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}
void CMesh::Render(IDirect3DDevice9* pDevice, D3DXVECTOR3 position, 
				   D3DXVECTOR3 rotation, D3DXVECTOR3 scale)
{
	D3DXMATRIX matTransform;
	D3DXMATRIX matRotation;
	D3DXMATRIX matTranslation;
	D3DXMATRIX matScale;
	
	D3DXMatrixIdentity(&matTransform);
	D3DXMatrixIdentity(&matRotation);
	D3DXMatrixIdentity(&matTranslation);
	D3DXMatrixIdentity(&matScale);

	D3DXMatrixTranslation(&matTranslation, position.x, position.y, position.z);
	D3DXMatrixRotationYawPitchRoll(&matRotation, rotation.y, rotation.x, rotation.z);
	D3DXMatrixScaling(&matScale, scale.x, scale.y, scale.z);

	matTransform = matScale * matRotation * matTranslation;
	pDevice->SetTransform(D3DTS_WORLD, &matTransform);
	for(int i = 0; i < m_vMaterials.size(); i++)
	{
		pDevice->SetMaterial( &m_vMaterials[i] );
		pDevice->SetTexture(0, m_vTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}