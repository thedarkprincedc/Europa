#pragma once
#include "DummyCamera.h"
class CDynamicCamera : public CDummyCamera
{
public:
	CDynamicCamera(void);
	~CDynamicCamera(void);
	void setLookAt(D3DXVECTOR3 &vector);
	void setPosition(D3DXVECTOR3 &vector);
	void setPosition(float x, float y, float z);
	void update(IDirect3DDevice9 *pDevice);
protected:
	CDummyCamera pDynamicCamera;

};
