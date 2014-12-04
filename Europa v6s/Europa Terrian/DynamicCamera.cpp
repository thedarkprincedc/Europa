#include "DynamicCamera.h"

CDynamicCamera::CDynamicCamera(void)
{
	//pDynamicCamera.x = 400;
}
CDynamicCamera::~CDynamicCamera(void)
{
}
void CDynamicCamera::setLookAt(D3DXVECTOR3 &vector)
{
	pDynamicCamera.setLookAt(vector);
}
void CDynamicCamera::setPosition(D3DXVECTOR3 &vector)
{
	this->x = pDynamicCamera.x = vector.x;
	this->y = pDynamicCamera.y = vector.y;
	this->z = pDynamicCamera.z = vector.z;
}
void CDynamicCamera::setPosition(float x, float y, float z)
{
	this->x = pDynamicCamera.x = x;
	this->y = pDynamicCamera.y = y;
	this->z = pDynamicCamera.z = z;
}
void CDynamicCamera::update(IDirect3DDevice9 *pDevice)
{
	D3DXVECTOR3 vector;
	D3DXVec3Subtract(&vector,this,&pDynamicCamera);
	D3DXVec3Normalize(&vector, &vector);
	D3DXVec3Add(&pDynamicCamera, &vector, &pDynamicCamera);
	pDynamicCamera.update(pDevice);
}
