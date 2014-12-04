#pragma once
#include <d3dx9.h>
class CDummyCamera : public D3DXVECTOR3
{
public:
	CDummyCamera(void);
	~CDummyCamera(void);
	virtual void setPosition(float x, float y, float z);
	virtual void setPosition(D3DXVECTOR3 &vector);
	void setRotation(D3DXVECTOR3 &vector);
	virtual void setLookAt(D3DXVECTOR3 &vector);
	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getRotation();
	D3DXVECTOR3 getLookAt();
	D3DXMATRIX *getViewMatrix(){	return &m_pViewTransform; }
	virtual void update(IDirect3DDevice9 *pDevice);
protected:
	D3DXVECTOR3 m_vUp;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vRotation;
	D3DXMATRIX  m_pViewTransform;
};
