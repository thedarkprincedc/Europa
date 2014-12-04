#include "DummyCamera.h"
#include <iostream>
using namespace std;
CDummyCamera::CDummyCamera(void)
{
	x = 0;
	y = 0;
	z = 0;
	m_vUp		= D3DXVECTOR3(0.0f,1.0f,0.0f);
	m_vRight	= D3DXVECTOR3(1.0f,0.0f,0.0f);
	m_vLook		= D3DXVECTOR3(0.0f,0.0f,1.0f);
	D3DXMatrixIdentity(&m_pViewTransform);
}
CDummyCamera::~CDummyCamera(void)
{
}
void CDummyCamera::setPosition(D3DXVECTOR3 &vector)
{
	this->x=vector.x;
	this->y=vector.y;
	this->z=vector.z;
}
void CDummyCamera::setPosition(float x, float y, float z)
{
	this->x=x;
	this->y=y;
	this->z=z;
}
void CDummyCamera::setRotation(D3DXVECTOR3 &vector)
{
	m_vRotation = vector;
}
void CDummyCamera::setLookAt(D3DXVECTOR3 &vector)
{
	m_vLook = vector;
}
D3DXVECTOR3 CDummyCamera::getPosition()
{
	return *this;
}
D3DXVECTOR3 CDummyCamera::getRotation()
{
	return m_vRotation;
}
D3DXVECTOR3 CDummyCamera::getLookAt()
{
	return m_vLook;
}
void CDummyCamera::update(IDirect3DDevice9 *pDevice)
{
	pDevice->GetTransform(D3DTS_VIEW, &m_pViewTransform);
	//D3DXMatrixTransformation(&m_pViewTransform, 0, 0, 0, 0, 0, (D3DXVECTOR3*)(this));
	D3DXMatrixTranslation(&m_pViewTransform, x, y, z);
	D3DXMatrixLookAtLH(&m_pViewTransform, this, &m_vLook, &m_vUp);
	pDevice->SetTransform(D3DTS_VIEW, &m_pViewTransform);
}