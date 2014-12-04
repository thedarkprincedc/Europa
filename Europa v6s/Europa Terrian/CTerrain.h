#pragma once
#include "../Europa v6/Core_Graphics.h"
#define getR(c) (((c)&0x00ff0000)>>16)
typedef struct {
	FLOAT x, y, z;
	DWORD color;
	FLOAT u, v;
}CUSTOM_FVF;
#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1)
class CTerrain{
public:
	CTerrain(IDirect3DDevice9 *pDevice, char *pszHeightMap){
		IDirect3DTexture9 *pHeightMap = GETCACHEDTEXTURE(pDevice, pszHeightMap, 0);
		GenerateTerrain(pDevice, pHeightMap); 
	}
	void GenerateTerrain(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *pHeightMap){
		BYTE HieghtMapArray[255][255]; // YAxis
		D3DLOCKED_RECT lockedrect;
		::ZeroMemory(&lockedrect, sizeof(lockedrect));
		if(FAILED(pHeightMap->LockRect(0, &lockedrect, NULL, 0))){
		}
		DWORD *pBits = (DWORD*)lockedrect.pBits;
		int index = 0;
		for(int y = 0; y < 256; y+=2){
			for(int x = 0; x < 256; x+=2){
				HieghtMapArray[y][x] = (getR(pBits[(y*256)+x]))/5;
			}
		}
		//
		int z = 128;
		float texX, texY;
		int nArrayIndex = 0;
		{
			for(int nIndexY = 0; nIndexY < 256; nIndexY += 2){
				texY = (float)(((float)nIndexY/(float)2)/(float)128);
				for(int nIndexX = 0; nIndexX < 255; nIndexX += 2){
					int x = nIndexX-128;
					texX = (float)(((float)nIndexX/(float)2)/(float)128);
					 // Vertice 1
					 m_vTerrain[((nIndexY/2) * 128)+(nIndexX/2)].x=(float)x;
					 m_vTerrain[((nIndexY/2) * 128)+(nIndexX/2)].y=(float)HieghtMapArray[nIndexY][nIndexX];
					 m_vTerrain[((nIndexY/2) * 128)+(nIndexX/2)].z=(float)z;

					 m_vTerrain[((nIndexY/2) * 128)+(nIndexX/2)].u=texX;
					 m_vTerrain[((nIndexY/2) * 128)+(nIndexX/2)].v=texY;

					 int col=HieghtMapArray[nIndexY][nIndexX]*5;
					 m_vTerrain[((nIndexY/2) * 128)+(nIndexX/2)].color=D3DCOLOR_ARGB(255, col, col, col);
				}
				z-=2;
			}
		}
		{
			 int nArrayIndex=0;
			 for(int nIndexY=0; nIndexY<128; nIndexY++)
			 {
				 for(int nIndexX=0; nIndexX<127; nIndexX++)
				 {
					 //int nIndexY=0;
					 m_sIndices[nArrayIndex]=((nIndexY+1) * 128)+nIndexX;
					 m_sIndices[nArrayIndex+1]=(nIndexY * 128)+nIndexX;
					 m_sIndices[nArrayIndex+2]=((nIndexY+1) * 128)+nIndexX+1;

					 m_sIndices[nArrayIndex+3]=((nIndexY+1) * 128)+nIndexX+1;
					 m_sIndices[nArrayIndex+4]=(nIndexY * 128)+nIndexX;
					 m_sIndices[nArrayIndex+5]=(nIndexY * 128)+nIndexX+1;

					 nArrayIndex+=6;
				 }
			 }
		}
		//pDevice->CreateVertexBuffer(4 * sizeof(CUSTOM_FVF), 0, PANEL_D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	
		 pDevice->CreateVertexBuffer(sizeof(m_vTerrain), 0, PANEL_D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
		 

		 // Now lock the vertex buffer...
		 BYTE* pVerticeLock=0;
		 //m_pVertexBuffer->Lock(0, 4*sizeof(CUSTOM_FVF), (VOID**)&pVertices, 0);
		 m_pVertexBuffer->Lock(0, sizeof(m_vTerrain), (VOID**)&pVerticeLock, 0);
	
		 // ...and copy our array of vertices straight into it
		 CopyMemory(pVerticeLock, &m_vTerrain, sizeof(m_vTerrain));

		 // Remember to unlock the vertex buffer once we're done.
		 m_pVertexBuffer->Unlock();


		 BYTE* pIndexData=0;

		 // Create the index buffer...
		 pDevice->CreateIndexBuffer(sizeof(m_sIndices), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIndexBuffer, 0);

		 // ...lock it
		 m_pIndexBuffer->Lock(0, sizeof(m_sIndices), (VOID**)&pIndexData, 0);

		 // ...copy the indicies in, and unlock
		 CopyMemory(pIndexData, &m_sIndices, sizeof(m_sIndices));
		 m_pIndexBuffer->Unlock();



	}
	void Render(IDirect3DDevice9 *pDevice){
		 HRESULT rslt;

		 // One of the simplest Render() methods yet!

		 //pDevice->SetVertexShader(PANEL_D3DFVF_CUSTOMVERTEX);
		 pDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);
		 // Use our nice ground texture for the entire set of vertices
//		 pDevice->SetTexture(0, m_pTextureGround);

		 // Set the texture as the first input for stage 0 
	//	 pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		 // Set the vertex diffuse colour as the second input for stage 0
	//	 pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		 // Set D3DTOP_BLENDDIFFUSEALPHA for the colour operation for stage 0
	//	 pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);

		 // Do not perform any operations for the alpha operation for stage 0
	//	 pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		 // Set the vertex stream source
		 pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CUSTOM_FVF));

		 // Tell D3D we're going to use our index buffer
		 pDevice->SetIndices(m_pIndexBuffer);

		 // Iterate and draw each row at a time
		 for(int nCount=0; nCount < 128; nCount++)
		 {
			pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 768, nCount*768, 256);
			
		 }

		 // Unset texture stage modes - these are the "default" D3D stages
		 pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);


	}
protected:
	IDirect3DVertexBuffer9	*m_pVertexBuffer;
	IDirect3DIndexBuffer9	*m_pIndexBuffer;
	CUSTOM_FVF				m_vTerrain[128*128];
	short					m_sIndices[(6*(128*128))];
};




//#define PANEL_D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_DIFFUSE)


IDirect3DVertexBuffer9	*m_pVertexBuffer;
IDirect3DTexture9 *pTexture;
void CreateGround(IDirect3DDevice9 *pDevice){
	pTexture = GETCACHEDTEXTURE(pDevice, "grass.jpg", 0);
	pDevice->CreateVertexBuffer(4 * sizeof(CUSTOM_FVF), 0, 
		PANEL_D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL);
	
	CUSTOM_FVF *pVertices = NULL;
	
	m_pVertexBuffer->Lock(0, 4*sizeof(CUSTOM_FVF), (VOID**)&pVertices, 0);
	
	pVertices[0].y = pVertices[1].y = pVertices[2].y = pVertices[3].y = 1.0f;
	
	float m_fLenght = 300;
	float m_fWidth = 300;
	D3DCOLOR m_dwColor = D3DCOLOR_XRGB(255, 255, 255);
	
	pVertices[0].x = -m_fWidth;
	pVertices[0].z = -m_fLenght;

	pVertices[1].x = -m_fWidth;
	pVertices[1].z = m_fLenght;
	
	pVertices[2].x = m_fWidth;
	pVertices[2].z = -m_fLenght;
	
	pVertices[3].x = m_fWidth;
	pVertices[3].z = m_fLenght;
	// Vertex Colors
	pVertices[0].color = m_dwColor;
	pVertices[1].color = m_dwColor;
	pVertices[2].color = m_dwColor;
	pVertices[3].color = m_dwColor;
	// Texture Coordinates
	pVertices[0].u = 0.0f;	// 1
	pVertices[0].v = 0.0f;	
	pVertices[1].u = 1.0f;	// 2
	pVertices[1].v = 0.0f;
	pVertices[2].u = 0.0f;  // 3
	pVertices[2].v = 1.0f;
	pVertices[3].u = 1.0f;	// 4
	pVertices[3].v = 1.0f;

	m_pVertexBuffer->Unlock();
}
void RenderGround(IDirect3DDevice9 *pDevice){
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE); //Problems w lightinf plain
	D3DXMATRIX dm;
	D3DXMatrixIdentity(&dm);
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CUSTOM_FVF));
	pDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);
	D3DXMatrixRotationX(&dm, 0.2f);
	::D3DXMatrixTranslation(&dm, 0, -30, 0);
	pDevice->SetTransform(D3DTS_WORLD, &dm);
	pDevice->SetTexture(0, pTexture);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	//pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}
