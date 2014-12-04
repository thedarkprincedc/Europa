#ifndef _CSPRITE_H_
#define _CSPRITE_H_
#include "./Global_Framework.h"
/***********************************************
*	CSprite - Used to Create Sprite Objects	   *
***********************************************/
class CSprite : public CPanelEx{
public:
	CSprite(IDirect3DDevice9 *pDevice, char *pImgFilename, 
		D3DCOLOR KeyColor = NULL, float x = 0, float y = 0);
	CSprite(IDirect3DDevice9 *pDevice, IDirect3DTexture9 *pTexture, 
		float x = 0, float y = 0);
	virtual ~CSprite();
	HRESULT Load(char *pImgFilename, D3DCOLOR color); // Load File
	HRESULT Load(IDirect3DTexture9 *pTexture);		  // Load Texture
	RECT GetRect();
	void Render(IDirect3DDevice9 *pDevice);
};

/*
class CSprite :  public CObject{
public:
	CSprite(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
			char *pImgFilename, D3DCOLOR color = NULL);
	CSprite(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite,
			IDirect3DTexture9 *pTexture, D3DCOLOR color = NULL);
	virtual ~CSprite();
	void Init();
	HRESULT Load(char *pImgFilename, D3DCOLOR color);// Load File
	HRESULT Load(IDirect3DTexture9 *pTexture);		 // Load Texture
	virtual void Render();							 // Render Sprite
	RITTI getRITTI();	// Get Runtime Type ID
	float getWidth();	// Get Width
	float getHeight();	// Get Height
	float getX();		// Get X
	float getY();		// Get Y
	RECT getRect();		// Get Rect
	//Transforms 
	void SetTranslation(D3DXVECTOR2 translation);	  // Set Position on X, Y Axis
	void SetTranslation(float x, float y);			  // 
	void SetTranslationRel(float x, float y){
		m_vTranslation.x += x;
		m_vTranslation.y += y;
	}
	void GetTranslation(D3DXVECTOR2 *translation);	  // Get Position on X, Y Axis	
	void SetRotation(float rotation);				  // Set Rotation 
	void SetRotationByDegree(float rotationDegree);	  // Set Rotation (degree)
	void GetRotation(float *rotation);				  // Get Rotation 
	void GetRotationByDegree(float *rotationDegree);  // Get Rotation (degree)
	void SetRotationPoint(D3DXVECTOR2 rotationPoint); // Set Rotation Point
	void GetRotationPoint(D3DXVECTOR2 *rotationPoint);// Get Rotation Point
	void SetScale(D3DXVECTOR2 scale);		// Set Scale
	void GetScale(D3DXVECTOR2 *scale);		// Get Scale
	// Colors 
	void SetColorKey(D3DCOLOR color);		// Set ColorKey
	void GetColorKey(D3DCOLOR *color);		// Get ColorKey
	void SetModulateColor(D3DCOLOR color);	// Set Modulate Color
	void GetModulateColor(D3DCOLOR *color);	// Get Modulate Color
protected:
	IDirect3DDevice9	*m_pDevice;		 //	Device
	ID3DXSprite			*m_pSprite;		 // Sprite Interface
	IDirect3DTexture9	*m_pTexture;	 //	Texture of Entity
	float				m_TextureWidth;	 //	Texture Width
	float				m_TextureHeight; //	Texture Height
	
	float				m_fRotation;	 //	Rotation
	D3DXVECTOR2			m_vRotationPoint;//	Rotation Point
	D3DXVECTOR2			m_vScale;		 //	Scale
	D3DXVECTOR2			m_vTranslation;	 //	Translation
	
	D3DCOLOR			m_ColorKey;		 //	Key Color for Transparency
	D3DCOLOR			m_ModulateColor; //	Moulate Color
	
	RECT				m_SourceRect;	 //
	D3DXVECTOR3			m_SourcePosition;//
};
*/
class CAnimTimer{
	long m_lastTime, m_TimeInterval;	// Timing Settings
public:
	CAnimTimer(long i);
	inline void SetTime(long i);
	inline BOOL Run();
};
/*
class CAnimSprite : public CSprite{
public:
	typedef enum {NONE, FORWARD, REVERSE, OSCILLATE} ANIMDIRECTION;
	RECT getRect(){
		RECT rc;
		ZeroMemory(&rc, sizeof(RECT));
		rc.top    = m_vTranslation.y; 
		rc.left	  = m_vTranslation.x;
		rc.right  = m_vTranslation.x + m_nFrameWidth;
		rc.bottom = m_vTranslation.y + m_nFrameHeight;
		return rc;
	};
protected:
	ANIMDIRECTION m_AnimationDirection;
	// Animation Members
	int m_OffsetX, m_OffsetY;			// Offset 
	int m_nFrameWidth, m_nFrameHeight;  // Frame Dimensions
	int m_nNumOfFrames, m_nCurrFrame;   // Frame Counts
	BOOL m_bPlayOnce;					// Times to Play
	// Animation Timing
	CAnimTimer *m_Timer;
	int nextframe;
	inline void SetOffsets();
public:
	CAnimSprite(IDirect3DDevice9 *pDevice, ID3DXSprite *pSprite, 
				char *pImgfilename, D3DCOLOR color = 0);
	void SetActiveSprite(int frameNum);
	void SetupAnimation(int numFrames, int frameWidth, int frameHeight, 
		BOOL playOnce, long timeIntervalMillisec, ANIMDIRECTION directionAnim);
	void Forward();
	void Reverse();
	void Oscillate();
	void Render();
};
*/
#endif