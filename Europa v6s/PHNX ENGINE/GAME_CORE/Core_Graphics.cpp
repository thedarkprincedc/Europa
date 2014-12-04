#include "Core_Global.h"

CGraphics::CGraphics(){
	m_hWnd		 = NULL;
	m_pD3D		 = NULL;
	m_pD3DDevice = NULL;
	m_pSprite	 = NULL;
	m_AmbientRed = m_AmbientGreen = m_AmbientBlue = 255;
	m_Width		 = 0;
	m_Height	 = 0;
	m_BPP		 = 0;
	m_bWindowed	 = TRUE;
	m_bZBuffer	 = FALSE;
	m_bHAL		 = FALSE;
	ZeroMemory(&m_d3ddm, sizeof(D3DDISPLAYMODE));
}

CGraphics::~CGraphics(){	Shutdown();		}

BOOL CGraphics::Init(){
	Shutdown();
	if((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return FALSE;
	return TRUE;
}

BOOL CGraphics::SetMode(HWND hWnd, BOOL windowed, BOOL useZbuffer,
						long width, long height, char BPP){
	D3DPRESENT_PARAMETERS	d3dpp;				// Direct3D Presentation Param
	D3DFORMAT				format, altformat;	// Direct3D Formats
	RECT					WndRect, ClientRect;// Client Rect
	long					WndWidth, WndHeight;// Window Width and Height
	//float					aspect;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	if((m_hWnd = hWnd) == NULL)	
		return FALSE;
	if(m_pD3D ==  NULL)			
		return FALSE;
	//	Get Current Direct3D Display Mode
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &m_d3ddm)))
		return FALSE;	
	/****************************
	*		Sets the Width		*
	****************************/
	if(!width){									//	if width is 0
		if(windowed == FALSE)						//	if in Fullscreen mode
			m_Width = m_d3ddm.Width;				//	match width to backsurface
		else{										//	if in windowed mode
			GetClientRect(m_hWnd, &ClientRect);			//	Match to client rectangle
			m_Width = ClientRect.right;
		}
	}
	else	m_Width = width;					//	if width > 0 then match to width
	/****************************
	*		Sets the Height		*
	****************************/
	if(!height){								//	if height is 0
		if(windowed == FALSE)						//	if in Fullscreen mode
			m_Height = m_d3ddm.Height;				//	match height to backsurface
		else{										//	if in windowed mode
			GetClientRect(m_hWnd, &ClientRect);			//	Match to client rectangle
			m_Height = ClientRect.bottom;
		}
	}
	else	m_Height = height;					//	if height > 0 then match to height
	
	if(!(m_BPP = BPP) || windowed == TRUE)
		if(!(m_BPP = GetFormatBPP(m_d3ddm.Format))) 
			return FALSE;
	if(windowed == TRUE){
		GetWindowRect(m_hWnd, &WndRect);
		GetClientRect(m_hWnd, &ClientRect);
		WndWidth = (WndRect.right - (ClientRect.right - m_Width)) - WndRect.left;
		WndHeight = (WndRect.bottom - (ClientRect.bottom - m_Height)) - WndRect.top;
		MoveWindow(m_hWnd, WndRect.left, WndRect.top, WndWidth, WndHeight, TRUE);
	}
//***********************************************************
//	D3DCAPS9 capabilities;
//	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &capabilities);
//	int vertexProcessing = 0;
//	if(capabilities.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
//		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
//	else	vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSIN
//*******************************************************************
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	m_bHAL = FALSE;
	if((m_bWindowed = windowed) == TRUE){
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		d3dpp.BackBufferFormat = m_d3ddm.Format;
		if(CheckFormat(m_d3ddm.Format, TRUE, TRUE) == TRUE){
			m_bHAL = TRUE;
		}
		else{
			if(CheckFormat(m_d3ddm.Format, TRUE, FALSE) == FALSE)
				return FALSE;
		}
	}
	else{
		d3dpp.Windowed = FALSE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_FLIP;
		d3dpp.BackBufferWidth = m_Width;
		d3dpp.BackBufferHeight = m_Height;
		d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
		//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		//d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		if(m_BPP == 32){
			format = D3DFMT_X8R8G8B8;
			altformat = D3DFMT_X8R8G8B8;
			//format = D3DFMT_A8R8G8B8;
			//altformat = D3DFMT_A8R8G8B8;
		}
		if(m_BPP == 24){
			format = D3DFMT_R8G8B8;
			altformat = D3DFMT_R8G8B8;
		}
		if(m_BPP == 16){
			format = D3DFMT_R5G6B5;
			altformat = D3DFMT_X1R5G5B5;
		}
		if(m_BPP == 8){
			format = D3DFMT_P8;
			altformat = D3DFMT_P8;
		}
		if(CheckFormat(format, FALSE, TRUE) == TRUE)
			m_bHAL = TRUE;
		else{
			if(CheckFormat(altformat, FALSE, TRUE) == TRUE){
				m_bHAL = TRUE;
				format = altformat;
			}
			else{
				if(CheckFormat(format, FALSE, FALSE) == FALSE){
					if(CheckFormat(altformat, FALSE, FALSE) == FALSE)
						return FALSE;
					else 
						format = altformat;
				}
			}
		}
		d3dpp.BackBufferFormat = format;
	}
	if((m_bZBuffer = useZbuffer) == TRUE){
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	}
	else{
		d3dpp.EnableAutoDepthStencil = FALSE;
	}
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			 (m_bHAL == TRUE) ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF,
			 hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, 
			 &m_pD3DDevice))){
		if(m_bZBuffer == TRUE){
			m_bZBuffer = FALSE;
			d3dpp.EnableAutoDepthStencil = FALSE;
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, 
					 (m_bHAL == TRUE) ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF,
					 hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp,
					 &m_pD3DDevice)))
				return FALSE;
		}
		else
			return FALSE;
	}
	//******************************************************
	/*D3DXVECTOR3 dir(1.0f, -1.0f, 1.0f);
	D3DXCOLOR col(1.0f, 1.0f, 1.0f, 1.0f);
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));
	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = col * 0.4f;
	light.Diffuse   = col;
	light.Specular  = col * 0.6f;
	light.Direction = dir;*/
	//m_pD3DDevice->SetLight(0, &light);
			m_pD3DDevice->LightEnable(0, true);
	//m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	//m_pD3DDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
	//########m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//******************************************************
	//EnableLighting(FALSE);
			EnableZBuffer(m_bZBuffer);
	//EnableAlphaBlending(FALSE);
	//EnableAlphaTesting(FALSE);

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	//m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	//m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

//	SetAmbientLight(255,255,255);

	//aspect = (float)m_Height / (float)m_Width;
	
	//SetPerspective(D3DX_PI/4, aspect, 1.0f, 10000.0f);
	
	if(FAILED(D3DXCreateSprite(m_pD3DDevice, &m_pSprite)))
		return FALSE;
	return TRUE;
}
BOOL CGraphics::Shutdown(){
	ReleaseCOM(m_pSprite);
	ReleaseCOM(m_pD3DDevice);
	ReleaseCOM(m_pD3D);
	return TRUE;
}
BOOL CGraphics::SetPerspective(float Fov, float Aspect, float Near,	float Far){
	D3DXMATRIX matProjection;
	
	if(m_pD3DDevice == NULL)	return FALSE;
	
	D3DXMatrixPerspectiveFovLH(&matProjection, Fov, Aspect, Near, Far);			
	
	if(FAILED(m_pD3DDevice->SetTransform(D3DTS_PROJECTION, &matProjection)))
		return FALSE;
	return TRUE; 
}
BOOL CGraphics::SetAmbientLight(char red, char green, char blue){
	D3DCOLOR color;
	
	if(m_pD3DDevice == NULL)	return FALSE;
	
	color = D3DCOLOR_XRGB((m_AmbientRed = red),
						  (m_AmbientGreen = green),
						  (m_AmbientBlue = blue));
	
	if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_AMBIENT, color)))
		return FALSE;

	return TRUE; 
}
BOOL CGraphics::GetAmbientLight(char *red, char *green, char *blue){
	if(red != NULL)		*red = m_AmbientRed;
	if(green != NULL)	*green = m_AmbientGreen;
	if(blue != NULL)	*blue = m_AmbientBlue;
	return TRUE; 
}
BOOL CGraphics::EnableLight(long num, BOOL Enable){
	if(m_pD3DDevice == NULL)	return FALSE;
	if(FAILED(m_pD3DDevice->LightEnable(num, Enable)))
		return FALSE;
	return TRUE; 
}
BOOL CGraphics::EnableLighting(BOOL Enable){
	if(m_pD3DDevice == NULL)	return FALSE;
	if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, Enable)))
		return FALSE;
	return TRUE; 
}
BOOL CGraphics::EnableZBuffer(BOOL Enable){
	if(m_pD3DDevice == NULL || m_bZBuffer == FALSE)
		return FALSE;
	if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_ZENABLE, 
		(Enable == TRUE) ? D3DZB_TRUE : D3DZB_FALSE)))
		return FALSE;
	return TRUE; 
}
BOOL CGraphics::EnableAlphaBlending(BOOL Enable, DWORD src, DWORD dest){
	if(m_pD3DDevice == NULL)
		return  FALSE;
	if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, Enable)))
		return FALSE;
	if(Enable == TRUE){
		m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, src);
		m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, dest);
	}
	return TRUE; 
}
BOOL CGraphics::EnableAlphaTesting(BOOL Enable){
	if(m_pD3DDevice == NULL)	
		return  FALSE;
	if(FAILED(m_pD3DDevice->SetRenderState(D3DRS_ALPHATESTENABLE, Enable)))
		return FALSE;
	if(Enable == TRUE){
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
		m_pD3DDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	}
	return TRUE; 
}
char CGraphics::GetFormatBPP(D3DFORMAT format){
	switch(format) {
		// 32 bit modes
		case D3DFMT_A8R8G8B8:
		case D3DFMT_X8R8G8B8:	
			return 32;
		// 24 bit modes
		case D3DFMT_R8G8B8:
			return 24;
		// 16 bit modes
		case D3DFMT_R5G6B5:
		case D3DFMT_X1R5G5B5:
		case D3DFMT_A1R5G5B5:
		case D3DFMT_A4R4G4B4:
			return 16;
		// 8 bit modes
		case D3DFMT_A8P8:
		case D3DFMT_P8:
			return 8;
		default:
			return 0;
	}
}
BOOL CGraphics::CheckFormat(D3DFORMAT Format, BOOL bWindowed, BOOL bHAL){
	if(FAILED(m_pD3D->CheckDeviceType(D3DADAPTER_DEFAULT,
				(bHAL == TRUE) ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF,
				Format, Format, bWindowed)))
		return FALSE;
	return TRUE;
}
BOOL CGraphics::Display(){
	if(m_pD3DDevice == NULL)
		return FALSE;
	if(FAILED(m_pD3DDevice->Present(NULL,NULL,NULL,NULL)))
		return FALSE;
	return TRUE;
}
BOOL CGraphics::BeginScene(){
	if(m_pD3DDevice == NULL)	
		return FALSE;
	if(FAILED(m_pD3DDevice->BeginScene()))	return FALSE;
	return TRUE;
}
BOOL CGraphics::EndScene(){
	if(m_pD3DDevice == NULL)
		return FALSE;
	if(FAILED(m_pD3DDevice->EndScene()))
		return FALSE;
	return TRUE;
}
BOOL CGraphics::Clear(long color, float zbuffer){
	if(m_pD3DDevice == NULL)
		return FALSE;
	if(m_bZBuffer == FALSE)
		return ClearDisplay(color);
	if(FAILED(m_pD3DDevice->Clear(0, NULL, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		color, zbuffer, 0)))
		return FALSE;
	return TRUE;
}
BOOL CGraphics::ClearDisplay(long color){
	if(m_pD3DDevice == NULL)
		return FALSE;
	if(FAILED(m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, color, 1.0f,0)))
		return FALSE;
	return TRUE;
}
IDirect3D9 *CGraphics::GetD3D(){
	return m_pD3D;
}
IDirect3DDevice9 *CGraphics::GetD3DDevice(){
	return m_pD3DDevice;
}
ID3DXSprite *CGraphics::GetSpriteObject(){
	return m_pSprite;
}
BOOL CGraphics::BeginSprite(){
	if(m_pSprite == NULL)	
		return FALSE;
	if(FAILED(m_pSprite->Begin(D3DXSPRITE_ALPHABLEND)))	
		return FALSE;
	return TRUE;
}
BOOL CGraphics::EndSprite(){
	if(m_pSprite == NULL)			
		return FALSE;
	if(FAILED(m_pSprite->End()))	
		return FALSE;
	return TRUE;
}
//CMesh::CMesh(){}