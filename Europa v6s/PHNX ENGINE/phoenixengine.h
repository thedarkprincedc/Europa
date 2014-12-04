#ifndef _PHOENIXENGINE_H_
#define _PHOENIXENGINE_H_
#include "./GAME_CORE/Core_Global.h"
#include "./GAME_FRAME/Global_Framework.h"
#include "./GAME_UTIL/GameEngineUtil.h"
static void SetUp2DCamera(IDirect3DDevice9 *pD3DDevice, float m_ScreenWidth, float m_ScreenHieght){//Orthorgonal Project
	D3DXMATRIX matOrtho, matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	D3DXMatrixOrthoLH(&matOrtho, m_ScreenWidth, m_ScreenHieght, 0.0f, 1.0f);
	pD3DDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	pD3DDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	pD3DDevice->SetTransform(D3DTS_VIEW, &matIdentity);
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
}
static void SetUp3DCamera(IDirect3DDevice9 *pD3DDevice){
    D3DXMATRIX matView;
    D3DXMatrixLookAtLH(&matView, 
		&D3DXVECTOR3(0.0f, 15.0f, -50.0f),	// Camera Position
        &D3DXVECTOR3(0.0f, 0.0f, 0.0f),		// Look At Position
        &D3DXVECTOR3(0.0f, 1.0f, 0.0f));	// Up Direction
    pD3DDevice->SetTransform(D3DTS_VIEW, &matView);
    D3DXMATRIX matProj;
    D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI/4, 1.25f, 1.0f, 2000.0f);
    pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProj);
	pD3DDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
#endif