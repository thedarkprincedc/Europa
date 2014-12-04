#include "Avatar.h"

CAvatar::CAvatar(void)
{
}
CAvatar::~CAvatar(void)
{
}
void CAvatar::load(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath)
{
//	turnTo(0,0,0);
	//Trans = srt
	bCtrlsEnabled = true;
	speed = 0.2f;
	pMesh = new CMesh(pDevice, pszFilename, pszTexturepath);
	dynamicChaseCamera = createVarCamera("AvatarDynamic", new CDynamicCamera);
	pMesh->setScale(0.002f);
	D3DXMatrixRotationYawPitchRoll(&pMesh->m_TransformMatrix, 0, (D3DX_PI/180)*-90, 0);
	pMesh->CalculateBoundingColisionBox();
	dynamicChaseCamera->setPosition(0,0,200);
	this->heading = D3DXVECTOR3(0,0,0);
	//dynamicChaseCamera->setPosition(D3DXVECTOR3(pMesh->x, pMesh->y + pMesh->maxBounds.y, pMesh->z+pMesh->minBounds.z));
}
void CAvatar::controls()
{
	UpdateControls();
	if(bCtrlsEnabled == true)
	{
	
		if(bLeft)
		{
			//pMesh->m_qRot.y += 10;
			pMesh->x++;
		}
		if(bRight)
		{

		}
		//pMesh->z -= m_fSpeed;
		//D3DXMatrixRotationY(&m_TransformMatrix, 80);
	//	dynamicChaseCamera->x = pMesh->x;
	//	dynamicChaseCamera->y = pMesh->y;
	//	dynamicChaseCamera->z = pMesh->z + pMesh->maxBounds.z;
		//dynamicChaseCamera->setLookAt(D3DXVECTOR3(pMesh->x, pMesh->y, pMesh->z));
		//D3DXQuaternionRotationYawPitchRoll(&pMesh->m_qRot, 0, pMesh->m_vRotation.x, pMesh->m_vRotation.z);
	}
}
void CAvatar::update(IDirect3DDevice9* pDevice)
{
	//status();
	controls();
	pMesh->Render(pDevice);
	dynamicChaseCamera->update(pDevice);
}
