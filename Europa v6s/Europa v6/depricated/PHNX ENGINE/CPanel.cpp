#include "./CPanel.h"
CPanelEx::CPanelEx(IDirect3DDevice9 *pDevice, float width, float height, 
	float ScreenWidth, float ScreenHeight, DWORD dwColor)
	:	m_pDevice(pDevice), m_pTexture(NULL), m_pVertexBuffer(NULL),
		m_nWidth(width), m_nHeight(height), m_nScreenWidth(ScreenWidth),
		m_nScreenHeight(ScreenHeight), m_dwColor(dwColor), m_x(0), m_y(0){
	if(CreateVertexBuffer())	// Creates the Vertex Buffer
		if(UpdateVertices())	// Add Vertices to a 
			return;
	
}
CPanel::CPanel(IDirect3DDevice9 *pDevice, IDirect3DTexture9	*pTexture,
			   int ScreenWidth, int ScreenHeight)
	:	m_pDevice(pDevice), m_pTexture(pTexture), 
		m_nScreenWidth(ScreenWidth), m_nScreenHeight(ScreenHeight){
	pTexture->AddRef();
	D3DSURFACE_DESC desc;				// Creates a surface discription var
	m_pTexture->GetLevelDesc(0, &desc); // Gets the textures description
	m_nWidth = desc.Width;				// sets the witdth 
	m_nHeight = desc.Height;			//	sets the height 
	if(CreateVertexBuffer())			// creates a vertex buffer
		if(UpdateVertices())			// Updates the vertex buffer with vertices
			return;
}
CPanelEx::~CPanelEx(){}
BOOL CPanelEx::SetTexture(char *pszTextureFilePath, DWORD dwKeyColor){
	// Create a Texture from file
	if(FAILED(D3DXCreateTextureFromFileEx(m_pDevice, pszTextureFilePath, 
			  0, 0, 0, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT,
			  D3DX_DEFAULT, dwKeyColor, NULL, NULL, &m_pTexture))){
		::OutputDebugString("Could not create Texture from file: ");
		::OutputDebugString(pszTextureFilePath);
		return FALSE;
	}
	return TRUE;
}
void CPanelEx::MoveTo(float x, float y){
	/*
		D3DXMATRIX matMove;											// Create a move Matrix 
		x -= (m_nScreenWidth/2) - (m_nWidth/2);						// Move X
		y -= (m_nScreenHeight/2) - (m_nHeight/2);					// Move Y
		// Set up Translation Matrix
		D3DXMatrixTranslation(&matMove,	(float)x, -(float)y, 0.0f); 
		m_pDevice->SetTransform(D3DTS_WORLD, &matMove);				// Move object
		m_x =x;
		m_y =y;
	*/
	m_x =x;
	m_y =y;
	x -= (m_nScreenWidth/2) - (m_nWidth/2);						// Move X
	y -= (m_nScreenHeight/2) - (m_nHeight/2);					// Move Y
	// Set up Translation Matrix
	//D3DXMatrixTranslation(&m_TransformMat, (float)x, -(float)y, 0.0f);
	CWorldTransform::TranslateAbs(x, -y, 0);
}
void CPanelEx::Render(){
	m_pDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(PANEL_CUSTOMVERTEX));
	m_pDevice->SetFVF(PANEL_D3DFVF_CUSTOMVERTEX);
	if(m_pTexture != NULL){
		m_pDevice->SetTexture(0, m_pTexture);
		m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	}
	else	
		m_pDevice->SetTexture(0, NULL);
	m_pDevice->SetTransform(D3DTS_WORLD, CWorldTransform::GetTransform());
	m_pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	// Draw the Rectangle in 
															// triangle strip mode
}


float CPanelEx::getWidth(){	return m_nWidth;	}
float CPanelEx::getHeight(){	return m_nHeight;	}
float CPanelEx::getX(){		return m_x;			}
float CPanelEx::getY(){		return m_y;			}
BOOL CPanelEx::CreateVertexBuffer(){
	if(FAILED(m_pDevice->CreateVertexBuffer(4*sizeof(PANEL_CUSTOMVERTEX),
						   0, PANEL_D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT,
						   &m_pVertexBuffer, NULL))){
		::OutputDebugString("Failed to Create Vertex Buffer");
		return FALSE;
	}
	return TRUE;
}
BOOL CPanelEx::UpdateVertices(){
	PANEL_CUSTOMVERTEX *pVertices = NULL;
	m_pVertexBuffer->Lock(0, 4 * sizeof(PANEL_CUSTOMVERTEX), (VOID**)&pVertices, 0);
	if(m_dwColor == -1)		m_dwColor = D3DCOLOR_XRGB(255,255,255);
	// Vertex Positions (Object Space)
	pVertices[0].x = -(m_nWidth) / 2.0f;
	pVertices[0].y = -(m_nHeight) / 2.0f;
	pVertices[0].z = 1.0f;
	pVertices[1].x = -(m_nWidth) / 2.0f;
	pVertices[1].y = (m_nHeight) / 2.0f;
	pVertices[1].z = 1.0f;
	pVertices[2].x = (m_nWidth) / 2.0f;
	pVertices[2].y = -(m_nHeight) / 2.0f;
	pVertices[2].z = 1.0f;
	pVertices[3].x = (m_nWidth) / 2.0f;
	pVertices[3].y = (m_nHeight) / 2.0f;
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