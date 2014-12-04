#include "CMeshPicker.h"
CMeshPicker::CMeshPicker(IDirect3DDevice9 *pDevice){
	//:	m_pDevice(pDevice){
	//m_pDevice->AddRef();
	//m_vCMesh.push_back(pMesh);
}
CMeshPicker::~CMeshPicker(){
	//m_pDevice->Release();
}
void CMeshPicker::Frame(){
	/*
	if(CXMesh *pMesh = CheckSelection(m_MousePos.x_pos, m_MousePos.y_pos)){
		if(_bLeft){
			OnLeftClick(pMesh);
			_bLeft = FALSE;
		}
		else
			OnMouseOver(pMesh);
	}*/
}
void CMeshPicker::OnMouseOver(CXMesh *pMesh){
	//static float x = 0;
	//x+= .005f;
	//pMesh->RotateRel(x, 0, 0);
}
void CMeshPicker::OnLeftClick(CXMesh *pMesh){
	/*static float oldX = 0;
	int mx = 0;
	if(oldX < m_MousePos.x_pos)
		mx = -1;
	else
		mx = 1;
	pMesh->TranslateRel(mx, 0, 0);
	 oldX = m_MousePos.x_pos;
	 */
}
void CMeshPicker::OnRightClick(CXMesh *pMesh){}
void CMeshPicker::TrackMouse(UINT uMsg, WPARAM wParam, LPARAM lParam){
/*	CXMesh *cpMesh;
	switch(uMsg){
		case WM_MOUSEMOVE:
			m_MousePos.x_pos = LOWORD(lParam);
			m_MousePos.y_pos = HIWORD(lParam);
		break;
		case WM_LBUTTONDOWN:	
			if((cpMesh = CheckSelection(LOWORD(lParam), HIWORD(lParam))) != NULL)
				OnLeftClick(cpMesh);
			_bLeft = TRUE;
		break;
	}*/
}
CXMesh *CMeshPicker::CheckSelection(int x, int y){
/*
	//Converts Mouse Pos into world space
	D3DVIEWPORT9 viewPort;
	m_pDevice->GetViewport(&viewPort);
	D3DXMATRIX matProj;
	D3DXVECTOR3 v;
	m_pDevice->GetTransform(D3DTS_PROJECTION, &matProj);
	//v.x = (((2.0f * m_MousePos.x_pos) / ScreenWidth) - 1) / matProj._11;
	//v.y = (((2.0f * m_MousePos.y_pos) / ScreenHeight) - 1) / matProj._22;
	//v.x = (((2.0f * x) / ScreenWidth) - 1) / matProj._11;
	//v.y = (((2.0f * y) / ScreenHeight) - 1) / matProj._22;
	v.x = (((2.0f * x) / viewPort.Width) - 1) / matProj._11;
	v.y = (((2.0f * y) / viewPort.Height) - 1) / matProj._22;
	v.z = 1.0f;

	// Creates the Ray
	D3DXMATRIX matView;
	m_pDevice->GetTransform(D3DTS_VIEW, &matView);
	D3DXMATRIX m;
	D3DXVECTOR3 rayOrgin, rayDir;
	D3DXMatrixInverse(&m, NULL, &matView);

	rayDir.x = v.x*m._11 + v.y*m._21 + v.z*m._31;
	rayDir.y = v.x*m._12 + v.y*m._22 + v.z*m._32;
	rayDir.z = v.x*m._13 + v.y*m._23 + v.z*m._33;
	rayOrgin.x = m._41;
	rayOrgin.y = m._42;
	rayOrgin.z = m._43;

	// 
	D3DXMATRIX matInverse, matWorld;
	m_pDevice->GetTransform(D3DTS_WORLD, &matWorld);
	D3DXMatrixInverse(&matInverse, NULL, &matWorld);
	D3DXVECTOR3 rayObjOrgin, rayObjDir;
	D3DXVec3TransformCoord(&rayObjOrgin,&rayOrgin,&matInverse);
	D3DXVec3TransformNormal(&rayObjDir,&rayDir,&matInverse);
	D3DXVec3Normalize(&rayObjDir, &rayObjDir);

	BOOL hasHit;
	float distanceToCollision;

	for(int i = 0; i < m_vCMesh.size(); ++i){
		if(!FAILED(D3DXIntersect(m_vCMesh[i]->GetD3DMesh(),&rayObjOrgin, &rayObjDir,
				   &hasHit, NULL, NULL, NULL, &distanceToCollision, NULL, NULL))){
			if(hasHit)
				return m_vCMesh[i];
		}
	}*/
	return NULL;
}