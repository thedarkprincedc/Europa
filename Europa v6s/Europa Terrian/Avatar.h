#pragma once
#include "DummyCamera.h"
#include "../Europa v6/CMesh.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include "CameraSystem.h"
#include "controls.h"
#include "ISteerable.h"
using namespace std;
class CAvatar : public ISteerable
{
public:
	CAvatar(void);
	virtual ~CAvatar(void);
public:
	void load(IDirect3DDevice9* pDevice, char *pszFilename, char *pszTexturepath = "./");
	void controls();
	void update(IDirect3DDevice9* pDevice);
protected:
	CMesh		 *pMesh;
	D3DXVECTOR3  velocity;
	D3DXVECTOR3	 heading; // degrees
	float		 speed;

	bool		 bCtrlsEnabled;
	CDummyCamera dummyChaseCamera;
	CDummyCamera *dynamicChaseCamera;
};