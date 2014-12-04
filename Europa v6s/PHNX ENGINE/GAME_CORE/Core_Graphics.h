/********************************************
	Core Graphics - Takes care of the d3d 
					init stuff. Supposed to
					beable to switch between 
					software and hardware 
					vetex processing modes
	Classes: CGraphics -> Functional
			 CMesh -> donot use not completed
					  use the CXMesh Class
********************************************/
#ifndef _CORE_GRAPHICS_H_
#define _CORE_GRAPHICS_H_
#include <Dxfile.h>
#define ReleaseCOM(x) if(x){x->Release(); x = NULL;}
class CGraphics;
class CMesh;
class CGraphics{
public:
	CGraphics();
	~CGraphics();
	IDirect3D9 *GetD3D();
	IDirect3DDevice9 *GetD3DDevice();
	ID3DXSprite	*GetSpriteObject();
	BOOL Init();
	BOOL Shutdown();
	BOOL SetMode(HWND hWnd, BOOL windowed = TRUE, BOOL useZbuffer = FALSE, 
				long width = 0, long height = 0, char BPP = 0);
	long GetNumDisplayModes(D3DFORMAT format);
	BOOL GetDisplayModeInfo(long num, D3DDISPLAYMODE *mode, D3DFORMAT format);
	char GetFormatBPP(D3DFORMAT format);
	BOOL CheckFormat(D3DFORMAT Format, BOOL bWindowed, BOOL bHAL);
	BOOL Display();
	BOOL BeginScene();
	BOOL EndScene();
	BOOL BeginSprite();
	BOOL EndSprite();
	BOOL Clear(long color = 0, float ZBuffer = 1.0f);
	BOOL ClearDisplay(long color = 0);
	BOOL ClearZBuffer(float ZBuffer = 1.0f);
	long GetHeight();
	long GetWidth();
	BOOL GetBPP();
	BOOL GetHAL();
	BOOL GetZBuffer();
	BOOL SetPerspective(float Fov = D3DX_PI / 4.0f, float Aspect = 1.3333f, 
						float Near = 1.0f, float Far = 10000.0f);
	BOOL SetAmbientLight(char red, char green, char blue);
	BOOL GetAmbientLight(char *red, char *green, char *blue);
	BOOL EnableLight(long num, BOOL Enable = TRUE);
	BOOL EnableLighting(BOOL Enable = TRUE);
	BOOL EnableZBuffer(BOOL Enable = TRUE);
	BOOL EnableAlphaBlending(BOOL Enable = TRUE, DWORD src = D3DBLEND_SRCALPHA, 
							DWORD dest = D3DBLEND_INVSRCALPHA);
	BOOL EnableAlphaTesting(BOOL Enable = TRUE);
protected:
	HWND				m_hWnd;
	IDirect3D9			*m_pD3D;
	IDirect3DDevice9	*m_pD3DDevice;
	ID3DXSprite			*m_pSprite;
	D3DDISPLAYMODE		m_d3ddm;
	BOOL				m_bWindowed;
	BOOL				m_bZBuffer;
	BOOL				m_bHAL;
	long				m_Width;
	long				m_Height;
	char				m_BPP;
	BYTE				m_AmbientRed;
	BYTE				m_AmbientGreen;
	BYTE				m_AmbientBlue;
};

typedef struct sMesh{
	char				*m_Name;
    ID3DXMesh			*m_Mesh;
	ID3DXMesh			*m_SkinMesh;
	ID3DXSkinInfo		*m_SkinInfo;
	
	DWORD				m_NumMaterials;
	D3DMATERIAL9		*m_Materials;
	IDirect3DTexture9	**m_Textures;

	DWORD				m_NumBones;
	D3DXMATRIX			*m_Matrices;
	D3DXMATRIX			**m_FrameMatrices;

	D3DXVECTOR3			m_Min, m_Max;
	float				m_Radius;
	
	sMesh				*m_Next;
	sMesh(){
		m_Name			= NULL;
		m_Mesh			= NULL;
		m_SkinMesh		= NULL;
		m_SkinInfo		= NULL;
		m_NumMaterials	= 0;
		m_Materials		= NULL;
		m_Textures		= NULL;
		m_NumBones		= 0;
		m_Matrices		= NULL;
		m_FrameMatrices = NULL;
		m_Min.x = m_Min.y = m_Min.z = m_Max.x = m_Max.y = m_Max.z = 0;
		m_Radius		= NULL;
		m_Next			= NULL;
	}
	~sMesh(){
		delete [] m_Name;
		ReleaseCOM(m_Mesh);
		ReleaseCOM(m_SkinMesh);
		ReleaseCOM(m_SkinInfo);
		delete [] m_Materials;
		if(m_Materials != NULL){
			for(DWORD i = 0; i < m_NumMaterials; i++){
				ReleaseCOM(m_Textures[i]);
			}
			delete [] m_Textures;
		}
		delete [] m_Matrices;
		delete [] m_FrameMatrices;
		delete m_Next;
	}
	sMesh *FindMesh(char *Name){
		sMesh *Mesh;
		if(Name == NULL)
			return this;
		if(m_Name != NULL && !strcmp(Name, m_Name))
			return this;
		if(m_Next != NULL){
			if((Mesh = m_Next->FindMesh(Name))!=NULL)
				return Mesh;
		}
		return NULL;
	}
	DWORD GetNumMaterials(){
		return m_NumMaterials;
	}
	D3DMATERIAL9 *GetMaterial(unsigned long Num){
		if(Num >= m_NumMaterials || m_Materials == NULL)
			return NULL;
		return &m_Materials[Num];
	}
	IDirect3DTexture9 *GetTexture(unsigned long Num){
		if(Num >= m_NumMaterials || m_Textures == NULL)
			return NULL;
		return m_Textures[Num];
	}
	void CopyFrameToBoneMatrices(){
		DWORD i;
		if(m_NumBones && m_Matrices != NULL && m_FrameMatrices != NULL){
			for(int i = 0; i < m_NumBones; i++){
				if(m_FrameMatrices[i] != NULL)
					D3DXMatrixMultiply(&m_Matrices[i], m_SkinInfo->GetBoneOffsetMatrix(i), 
						m_FrameMatrices[i]);
				else
					D3DXMatrixIdentity(&m_Matrices[i]);
			}
		}
		if(m_Next != NULL)
			m_Next->CopyFrameToBoneMatrices();
	}
}sMesh;

typedef struct{
	DWORD Time;
	DWORD Floats;
	float w;
	float x;
	float y;
	float z;
}sXFileRotateKey;
typedef struct{
	DWORD Time;
	DWORD Floats;
	D3DXVECTOR3 Scale;
}sXFileScaleKey;
typedef struct{
	DWORD Time;
	DWORD Floats;
	D3DXVECTOR3 Pos;
}sXFilePositionKey;
typedef struct{
	DWORD Time;
	DWORD Floats;
	D3DXMATRIX Matrix; 
}sXFileMatrixKey;
typedef struct{
	DWORD Time;
	D3DXQUATERNION Quarternion;
}sRotateKey;

typedef struct{
	DWORD Time;
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 PosInterpolation;
}sPositionKey;
typedef struct{
	DWORD Time;
	D3DXVECTOR3 Scale;
	D3DXVECTOR3 ScaleInterpolation;
}sScaleKey;
typedef struct{
	DWORD Time;
	D3DXMATRIX Matrix;
	D3DXMATRIX MatInterpolation;
}sMatrixKey;

typedef struct sFrameMeshList{
	sMesh			*m_Mesh;
	sFrameMeshList  *m_Next;
	sFrameMeshList(){
		m_Mesh = NULL;
		m_Next = NULL;
	}
	~sFrameMeshList(){
		delete m_Next;
	}
}sFrameMeshList;
typedef struct sFrame{
	char			*m_Name;
	sFrameMeshList  *m_MeshList;
	D3DXMATRIX		 m_matCombined;
	D3DXMATRIX		 m_matTransformed;
	D3DXMATRIX		 m_matOriginal;
	sFrame			*m_Parent;
	sFrame			*m_Child;
	sFrame			*m_Sibling;

	sFrame(){
		m_Name = NULL;
		m_MeshList = NULL;
		D3DXMatrixIdentity(&m_matCombined);
		D3DXMatrixIdentity(&m_matTransformed);
		D3DXMatrixIdentity(&m_matOriginal);
		m_Parent = m_Child = m_Sibling = NULL;
	}
	~sFrame(){
		delete m_Name;
		delete m_MeshList;
		delete m_Child;
		delete m_Sibling;
	}
	sFrame *FindFrame(char *Name){
		sFrame *Frame;
		if(Name == NULL)
			return this;
		if(m_Name != NULL && !strcmp(Name, m_Name))
			return this;
		if(m_Child != NULL){
			if((Frame = m_Child->FindFrame(Name)) != NULL)
				return Frame;
		}
		if(m_Sibling != NULL){
			if((Frame = m_Sibling->FindFrame(Name)) != NULL)
				return Frame;
		}
		return NULL;
	}
	void ResetMatrices(){
		m_matTransformed = m_matOriginal;
		if(m_Child != NULL)
			m_Child->ResetMatrices();
		if(m_Sibling != NULL)
			m_Sibling->ResetMatrices();
	}
	void AddMesh(sMesh *Mesh){
		sFrameMeshList *List;
		List = new sFrameMeshList();
		List->m_Mesh = Mesh;
		List->m_Next = m_MeshList;
		m_MeshList = List;
	}
}sFrame;
class CMesh{
private:
	CGraphics	m_Graphics;
	long		m_NumMeshes;
	sMesh		m_Meshes;
	long		m_NumFrames;
	sFrame		m_Frames;
	D3DXVECTOR3 m_Min, m_Max;
	float		m_Radius;
	void ParseXFileData(IDirectXFileData *pData, sFrame *ParentFrame, char *TexturePath =".\\");
	void MapFramesToBones(sFrame *Frame);
public:
	CMesh();
	~CMesh();
	BOOL IsLoaded();
	long GetNumFrames();
	sFrame *GetParentFrame();
	sFrame *GetFrame(char *Name);
	long GetNumMeshes();
	sMesh *GetParentMesh();
	sMesh *GetMesh(char *Name);
	BOOL GetBounds(float *MinX, float *MinY, float *MinZ,
		float *MaxX, float *MaxY, float *MaxZ, float *Radius);
	BOOL Load(CGraphics *Graphics, char *Filename, char *TexturePath = ".\\");
	BOOL Free();
};

typedef struct sAnimation{
	char	*m_Name;
	char	*m_FrameName;
	sFrame  *m_Frame;

	BOOL	m_Loop;
	BOOL	m_Linear;

	DWORD			 m_NumPositionKeys;
	sPositionKey	*m_PositionKeys;

	DWORD			 m_NumRotateKeys;
	sRotateKey		*m_RotateKeys;

	DWORD			 m_NumScaleKeys;
	sScaleKey		*m_ScaleKeys;

	DWORD			 m_NumMatrixKeys;
	sMatrixKey		*m_MatrixKeys;

	sAnimation		*m_Next;

	sAnimation(){
		m_Name		= NULL;
		m_FrameName = NULL;
		m_Frame		= NULL;
		m_Loop		= FALSE;
		m_Linear	= TRUE;
		m_NumPositionKeys = 0;
		m_PositionKeys	  = NULL;
		m_NumRotateKeys	  = 0;
		m_RotateKeys	  = NULL;
		m_NumScaleKeys	  = 0;
		m_ScaleKeys		  = NULL;
		m_NumMatrixKeys	  = 0;
		m_MatrixKeys	  = NULL;
		m_Next			  = NULL;
	}
	~sAnimation(){
		delete [] m_Name;			m_Name         = NULL;
		delete [] m_FrameName;		m_FrameName    = NULL;
		delete [] m_PositionKeys;	m_PositionKeys = NULL;
		delete [] m_RotateKeys;		m_RotateKeys = NULL;
		delete [] m_ScaleKeys;		m_ScaleKeys	   = NULL;
		delete [] m_MatrixKeys;		m_MatrixKeys   = NULL;
		delete m_Next;				m_Next         = NULL;				
	}
	void Update(DWORD Time, BOOL Smooth){
		unsigned long i, Key;
		DWORD TimeDiff, IntTime;
		D3DXMATRIX Matrix, MatTemp;
		D3DXVECTOR3 Vector;
		D3DXQUATERNION Quat;
		
		if(m_Frame == NULL)
			return;
		if(m_NumRotateKeys || m_NumScaleKeys || m_NumPositionKeys){
			D3DXMatrixIdentity(&Matrix);
			if(m_NumRotateKeys && m_RotateKeys != NULL){
				Key = 0;
				for(int i = 0; i < m_NumRotateKeys; i++){
					if(m_RotateKeys[i].Time <=  Time)
						Key = i;
					else
						break;
				}
				if(Key == (m_NumRotateKeys-1) || Smooth == FALSE){
					Quat = m_RotateKeys[Key].Quarternion;
				}
				else{
					TimeDiff = m_RotateKeys[Key+1].Time - m_RotateKeys[Key].Time;
					IntTime = Time - m_RotateKeys[Key].Time;
					D3DXQuaternionSlerp(&Quat, &m_RotateKeys[Key].Quarternion,
						&m_RotateKeys[Key+1].Quarternion,((float)IntTime / (float)TimeDiff));
				}
				D3DXMatrixRotationQuaternion(&MatTemp, &Quat);
				D3DXMatrixMultiply(&Matrix, &Matrix, &MatTemp);
			}
			if(m_NumScaleKeys && m_ScaleKeys != NULL){
				Key = 0;
				for(int i = 0; i < m_NumScaleKeys; i++){
					if(m_ScaleKeys[i].Time <= Time)
						Key = i;
					else 
						break;
				}
				if(Key == (m_NumScaleKeys-1) || Smooth == FALSE){
					Vector = m_ScaleKeys[Key].Scale;
				}
				else {
					IntTime = Time - m_ScaleKeys[Key].Time;
					Vector = m_ScaleKeys[Key].Time + 
						m_ScaleKeys[Key].ScaleInterpolation * (float)IntTime;
				}
				D3DXMatrixScaling(&MatTemp, Vector.x, Vector.y, Vector.z);
				D3DXMatrixMultiply(&Matrix, &Matrix, &MatTemp);
			}
			if(m_NumPositionKeys && m_PositionKeys != NULL){
				Key = 0;
				for(int i = 0; i < m_NumPositionKeys; i++){
					if(m_PositionKeys[i].Time <= Time)
						Key = i;
					else
						break;
				}
				if(Key = (m_NumPositionKeys-1) || Smooth == FALSE){
					Vector  = m_PositionKeys[Key].Pos;
				}
				else{
					IntTime = Time - m_PositionKeys[Key].Time;
					Vector = m_PositionKeys[Key].Pos + 
						m_PositionKeys[Key].PosInterpolation * (float)IntTime;
				}
				D3DXMatrixTranslation(&MatTemp, Vector.x, Vector.y, Vector.z);
				D3DXMatrixMultiply(&Matrix, &Matrix, &MatTemp);
			}
			m_Frame->m_matTransformed = Matrix;
		}
		if(m_NumMatrixKeys && m_MatrixKeys != NULL){
			Key = 0;
			for(int i = 0; i < m_NumMatrixKeys; i++){
				if(this->m_MatrixKeys[i].Time <= Time)
					Key = i;
				else
					break;
			}
			if(Key == (m_NumMatrixKeys-1) || Smooth == FALSE){
				//*******************************************
				m_Frame->m_matTransformed = m_MatrixKeys[Key].Matrix;
			}
			else{
				IntTime = Time - m_MatrixKeys[Key].Time;
				Matrix = m_MatrixKeys[Key].MatInterpolation * (float)IntTime;
				m_Frame->m_matTransformed = Matrix + m_MatrixKeys[Key].Matrix;
			}
		}
	}
}sAnimation;
typedef struct sAnimationSet{
	char		  *m_Name;
	sAnimation    *m_Animation;
	unsigned long  m_Length;
	sAnimationSet *m_Next;
	sAnimationSet(){
		m_Name = NULL;
		m_Animation = NULL;
		m_Length = 0;
		m_Next = NULL;
	}
	~sAnimationSet(){
		delete [] m_Name;    m_Name = NULL;
		delete m_Animation;  m_Animation = NULL; 
		delete m_Next;       m_Next = NULL;
	}
	sAnimationSet *FindSet(char *Name){
		sAnimationSet *AnimSet;
		if(Name == NULL)
			return this;
		if(m_Name != NULL && !strcmp(Name, m_Name))
			return this;
		if(m_Next != NULL){
			if((AnimSet = m_Next->FindSet(Name)) != NULL)
				return AnimSet;
		}
		return NULL;
	}
	void Update(DWORD Time, BOOL Smooth){
		sAnimation *Anim;
		Anim = m_Animation;
		while(Anim != NULL){
			if(!m_Length)
				Anim->Update(0, FALSE);
			else
				if(Time >= m_Length && Anim->m_Loop == FALSE)
					Anim->Update(Time, FALSE);
				else
					Anim->Update((Time % m_Length), TRUE);
			Anim = Anim->m_Next;
		}
	}
}sAnimationSet;
class CAnimation{
protected:
	long		   m_NumAnimations;
	sAnimationSet *m_AnimationSet;
	void ParseXFileData(IDirectXFileData *DataObj, 
		sAnimationSet *ParentAnim,
		sAnimation *CurrentAnim);
public:
	CAnimation();
	~CAnimation();
	BOOL isLoaded();
	long GetNumAnimations();
	sAnimationSet *GetAnimationSet(char *Name = NULL);
	unsigned long GetLength(char *Name = NULL);
	BOOL Load(char *Filename, CMesh *MapMesh = NULL);
	BOOL Free();
	BOOL MapToMesh(CMesh *Mesh);
	BOOL SetLoop(BOOL ToLoop, char *Name = NULL);
};

#endif