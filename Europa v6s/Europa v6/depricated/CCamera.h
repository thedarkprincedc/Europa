/********************************
	CCamera Class - Takes care of Camera Movement
					by hijacking the view matrix
	BrettMosley@hotmail.com
*********************************/
#pragma once
#include <d3dx9.h>

#define DEFAULT_POSITION D3DXVECTOR3(0.0f,0.0f,0.0f)
#define DEFAULT_RIGHT	 D3DXVECTOR3(1.0f,0.0f,0.0f)
#define DEFAULT_UP		 D3DXVECTOR3(0.0f,1.0f,0.0f)
#define DEFAULT_LOOK	 D3DXVECTOR3(0.0f,0.0f,1.0f)
enum CameraType	{	AIRCRAFT, LANDOBJECT	};
class CCamera{
public:
	void SetCameraType(CameraType cameraType);
public:
	CCamera(IDirect3DDevice9 *pDevice, CameraType cameraType);
	~CCamera();
	virtual void Frame();
	virtual void Controls();

	void Walk(float units);	  // Forward / Backward
	void Strafe(float units); // Left / Right
	void Fly(float units);	  // Up / Down
	void Pitch(float angle);  // Rotate right vector
	void Yaw(float angle);	  // Rotate up vector
	void Roll(float angle);	  // Rotate look vector
	
	void GetViewMatrix(D3DXMATRIX *viewMatix);
	void GetRightVector(D3DXVECTOR3 *right);
	void GetUpVector(D3DXVECTOR3 *up);
	void GetLookVector(D3DXVECTOR3 *look);
	void GetPosition(D3DXVECTOR3 *position);
	void SetPosition(D3DXVECTOR3 *position);
protected:
	IDirect3DDevice9	*m_pDevice;
	CameraType			 m_CameraType;
	D3DXVECTOR3			 m_vUp;
	D3DXVECTOR3			 m_vRight;
	D3DXVECTOR3			 m_vLook;
	D3DXVECTOR3			 m_vPosition;
};