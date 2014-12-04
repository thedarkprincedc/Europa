#include "./Global_Framework.h"
/*
CPanelEx::CPanelEx(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *pTexture)
	:	m_pTexture(pTexture), m_pVertexBuffer(NULL), m_fx(0), m_fy(0){
	pTexture->AddRef();
	D3DSURFACE_DESC desc;				// Creates a surface discription var
	m_pTexture->GetLevelDesc(0, &desc); // Gets the textures description
	m_fWidth  = desc.Width;				// sets the witdth 
	m_fHeight = desc.Height;			// sets the height 
	m_fRealY = 0; 
	if(CreateVertexBuffer(pDevice))		// creates a vertex buffer
		if(UpdateVertices())			// Updates the vertex buffer with vertices
			return;
}

CPanelEx::CPanelEx(IDirect3DDevice9 *pDevice, char *pszFilename, D3DCOLOR KeyColor, float x, float y)
	:	m_pTexture(NULL), m_pVertexBuffer(NULL), m_fx(0), m_fy(0){
	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);
	m_nScreenWidth  = viewPort.Width;
	m_nScreenHeight = viewPort.Height;
	SetTexture(pDevice, pszFilename, KeyColor);
	m_fRealY = y;
	TranslateAbs(x, y);
	if(CreateVertexBuffer(pDevice))			// creates a vertex buffer
		if(UpdateVertices())			// Updates the vertex buffer with vertices
			return;
}
CPanelEx::CPanelEx(IDirect3DDevice9 *pDevice, float width, float height, DWORD dwColor)
	:	m_pTexture(NULL), m_pVertexBuffer(NULL), m_fWidth(width), 
	    m_fHeight(height), m_dwColor(dwColor), m_fx(0), m_fy(0){
	D3DVIEWPORT9 viewPort;
	pDevice->GetViewport(&viewPort);
	m_nScreenWidth = viewPort.Width;
	m_nScreenHeight = viewPort.Height;
	m_fRealY = 0;
	if(CreateVertexBuffer(pDevice))	// Creates the Vertex Buffer
		if(UpdateVertices())	// Add Vertices to a 
			return;
}
IDirect3DTexture9 *CPanelEx::GetTexture(){ return m_pTexture; }
IDirect3DVertexBuffer9 *CPanelEx::GetVertexBuffer(){ return m_pVertexBuffer; }
CPanelEx::~CPanelEx(){	
	//if(m_pTexture)	
	//	m_pTexture->Release(); 
}
BOOL CPanelEx::SetTexture(IDirect3DDevice9 *pDevice, char *pszTextureFilePath, 
						  DWORD dwKeyColor){
	// Create a Texture from file
	//if(FAILED(D3DXCreateTextureFromFileEx(pDevice, pszTextureFilePath, 
	//		  0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
	//		  D3DX_DEFAULT, dwKeyColor, NULL, NULL, &m_pTexture))){
	//	OutputDebugString("Could not create Texture from file: ");
	//	OutputDebugString(pszTextureFilePath);
	//	return FALSE;
	//}
    m_pTexture = GETCACHEDTEXTURE(pDevice, pszTextureFilePath, dwKeyColor);
	D3DSURFACE_DESC desc;				// Creates a surface discription var
	m_pTexture->GetLevelDesc(0, &desc); // Gets the textures description
	m_fWidth = desc.Width;				// sets the witdth 
	m_fHeight = desc.Height;			//	sets the height 
	
	CreateVertexBuffer(pDevice);			// creates a vertex buffer
	UpdateVertices();			// Updates the vertex buffer with vertices
			
	return TRUE;
}
void CPanelEx::TranslateAbs(float x, float y){
	m_fx = x;
	m_fy = y;
	m_fRealY = y;
	x -= (m_nScreenWidth/2) - (m_fWidth/2);		// Move X
	y -= (m_nScreenHeight/2) - (m_fHeight/2);	// Move Y
	CWorldTransform::TranslateAbs(x, -y, 0);
}
void CPanelEx::TranslateRel(float x, float y){
	int tx = GetXPosition();
	int ty = GetYPosition();
	m_fRealY += -y;
	m_fx += x;
	m_fy += y;
	CWorldTransform::TranslateAbs(tx+x, (ty+y), 0);
}
void CPanelEx::Render(IDirect3DDevice9 *pDevice){
	// Set Vertx Stream Source 
	pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(PANEL_CUSTOMVERTEX));
	pDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);	// Set Flexable Vertex Format
	if(m_pTexture != NULL){
		pDevice->SetTexture(0, m_pTexture);				 // Set Texture 
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, // Set Texture Stage State
			D3DTOP_MODULATE); 
	}
	else	
		pDevice->SetTexture(0, NULL);	
	if(m_dwColor != NULL){
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	}
	pDevice->SetTransform(D3DTS_WORLD, CWorldTransform::GetTransform());
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	// Draw the Rectangle in 
														// triangle strip mode
	if(m_dwColor != NULL){
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
		pDevice->SetRenderState(D3DRS_SRCBLEND,D3DBLEND_ONE);
		pDevice->SetRenderState(D3DRS_DESTBLEND,D3DBLEND_ZERO);
	}
}
float CPanelEx::GetScreenWidth(){ return m_nScreenWidth; }
float CPanelEx::GetScreenHeight(){ return m_nScreenHeight; }
float CPanelEx::GetWidth(){	return m_fWidth; }
float CPanelEx::GetHeight(){ return m_fHeight; }
float CPanelEx::GetX(){	return m_fx; }
float CPanelEx::GetY(){	
	//return m_fy; 
	return m_fRealY; //Temporary fix
}
BOOL CPanelEx::CreateVertexBuffer(IDirect3DDevice9 *pDevice){
	if(FAILED(pDevice->CreateVertexBuffer(4*sizeof(PANEL_CUSTOMVERTEX), 0, 
		PANEL_D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &m_pVertexBuffer, NULL))){
		OutputDebugString("Failed to Create Vertex Buffer");
		return FALSE;
	}
	return TRUE;
}
BOOL CPanelEx::UpdateVertices(){
	PANEL_CUSTOMVERTEX *pVertices = NULL;
	m_pVertexBuffer->Lock(0, 4 * sizeof(PANEL_CUSTOMVERTEX), (VOID**)&pVertices, 0);
	if(m_dwColor == -1)	m_dwColor = D3DCOLOR_XRGB(255,255,255);
	// Vertex Positions (Object Space)
	pVertices[0].x = -(m_fWidth) / 2.0f;
	pVertices[0].y = -(m_fHeight) / 2.0f;
	pVertices[0].z = 1.0f;
	pVertices[1].x = -(m_fWidth) / 2.0f;
	pVertices[1].y = (m_fHeight) / 2.0f;
	pVertices[1].z = 1.0f;
	pVertices[2].x = (m_fWidth) / 2.0f;
	pVertices[2].y = -(m_fHeight) / 2.0f;
	pVertices[2].z = 1.0f;
	pVertices[3].x = (m_fWidth) / 2.0f;
	pVertices[3].y = (m_fHeight) / 2.0f;
	pVertices[3].z = 1.0f;
	// Vertex Colors
	pVertices[0].color = m_dwColor;
	pVertices[1].color = m_dwColor;
	pVertices[2].color = m_dwColor;
	pVertices[3].color = m_dwColor;
	// Texture Coordinates
	pVertices[0].u = 0.0f;	// 1
	pVertices[0].v = 1.0f;	
	pVertices[1].u = 0.0f;	// 2
	pVertices[1].v = 0.0f;
	pVertices[2].u = 1.0f;  // 3
	pVertices[2].v = 1.0f;
	pVertices[3].u = 1.0f;	// 4
	pVertices[3].v = 0.0f;
	m_pVertexBuffer->Unlock();
	return TRUE;
}
*/